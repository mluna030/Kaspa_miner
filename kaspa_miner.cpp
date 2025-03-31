#ifndef __SYNTHESIS__
#include "sim_ap_int.h"
#pragma message("✅ using sim_ap_int in kaspa_miner.cpp")
#else
#include <ap_int.h>
#endif

#include <stdint.h>

typedef ap_uint<8> byte;
typedef ap_uint<32> u32;
typedef ap_uint<256> hash256_t;

#define HEADER_SIZE 80
#define HASH_SIZE 32
#define NONCE_TRIALS 1024

void heavyhash_core(hash256_t hash_in, uint64_t timestamp, uint64_t nonce, byte digest_out[32]);

extern "C" {
void kaspa_miner(
    volatile byte *header,           
    hash256_t target,                
    u32 start_nonce,                 
    u32 *found_nonce,                
    ap_uint<1> *found                
) {
#ifdef __SYNTHESIS__
#pragma HLS INTERFACE m_axi port=header offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=target bundle=control
#pragma HLS INTERFACE s_axilite port=start_nonce bundle=control
#pragma HLS INTERFACE s_axilite port=found_nonce bundle=control
#pragma HLS INTERFACE s_axilite port=found bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
#endif

    byte local_header[HEADER_SIZE];
#ifdef __SYNTHESIS__
#pragma HLS ARRAY_PARTITION variable=local_header complete dim=1
#endif

for (int i = 0; i < HEADER_SIZE; i++) {
    #ifdef __SYNTHESIS__
    #pragma HLS UNROLL
    #endif
        local_header[i] = static_cast<byte>(header[i]);
    }

    *found = 0;

    for (int i = 0; i < NONCE_TRIALS; i++) {
#ifdef __SYNTHESIS__
#pragma HLS PIPELINE II=1
#endif
        u32 nonce = start_nonce + i;

        hash256_t hash_input = 0;
        for (int b = 0; b < 32; b++) {
            hash_input.set_range(b * 8 + 7, b * 8, local_header[b]);
        }

        uint64_t timestamp = 0;
        for (int j = 0; j < 8; j++) {
            timestamp |= ((uint64_t)local_header[32 + j]) << (8 * j);
        }

        byte digest[32];
        heavyhash_core(hash_input, timestamp, nonce, digest);

        hash256_t digest_val = 0;
        for (int b = 0; b < 32; b++) {
            digest_val.set_range(b * 8 + 7, b * 8, digest[b]);
        }

        if (digest_val < target) {
            *found = 1;
            *found_nonce = nonce;
            break;
        }
    }
}
}
