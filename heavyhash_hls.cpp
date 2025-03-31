#include "xoshiro256pp.h"
#include <ap_int.h>
#include <stdint.h>

#define SIZE 64
#define ITERATIONS (SIZE / 4)

typedef ap_uint<8> u8;
typedef ap_uint<16> u16;
typedef ap_uint<64> u64;
typedef ap_uint<256> hash256_t;

typedef u16 matrix[SIZE][SIZE];

// Matrix generation
void generate_matrix(matrix mat, hash256_t input_hash) {
    Xoshiro256PP rng;
    rng.init(
        input_hash.range(63, 0),
        input_hash.range(127, 64),
        input_hash.range(191, 128),
        input_hash.range(255, 192)
    );

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j += ITERATIONS) {
#pragma HLS PIPELINE
            u64 value = rng.next();
            for (int k = 0; k < ITERATIONS; k++) {
#pragma HLS UNROLL
                mat[i][j + k] = (value >> (4 * k)) & 0x0F;
            }
        }
    }
}

// HeavyHash core function
void heavyhash_core(
    hash256_t hash_in,
    u64 timestamp,
    u64 nonce,
    u8 digest_out[32]
) {
#pragma HLS INLINE off

    matrix mat;
    generate_matrix(mat, hash_in);

    // Build header = hash (32) + timestamp (8) + padding (32) + nonce (8)
    u8 header[80] = {0};
    for (int i = 0; i < 32; i++) header[i] = hash_in.range(i * 8 + 7, i * 8);
    for (int i = 0; i < 8; i++) header[32 + i] = (timestamp >> (8 * i)) & 0xFF;
    for (int i = 0; i < 8; i++) header[72 + i] = (nonce >> (8 * i)) & 0xFF;

    // Simulated cSHAKE256 — we'll replace with real Blake3 later
    for (int i = 0; i < 32; i++) {
        header[i] ^= header[72 + (i % 8)] ^ (i * 37);
    }

    // Build 4-bit vector v[]
    u16 v[SIZE];
    for (int i = 0; i < SIZE / 2; i++) {
        v[2 * i] = (header[i] >> 4) & 0x0F;
        v[2 * i + 1] = header[i] & 0x0F;
    }

    // Matrix-vector multiply p[i] = (mat[i] * v) >> 10
    u16 p[SIZE];
    for (int i = 0; i < SIZE; i++) {
        u32 sum = 0;
        for (int j = 0; j < SIZE; j++) {
#pragma HLS UNROLL
            sum += mat[i][j] * v[j];
        }
        p[i] = sum >> 10;
    }

    // Digest = header[i] XOR packed 4-bit values from p[]
    for (int i = 0; i < 32; i++) {
        digest_out[i] = header[i] ^ ((p[i * 2] << 4) | (p[i * 2 + 1] & 0x0F));
    }

    // Reverse bytes in-place
    for (int i = 0; i < 16; i++) {
        u8 tmp = digest_out[i];
        digest_out[i] = digest_out[31 - i];
        digest_out[31 - i] = tmp;
    }
}

