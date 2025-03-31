#ifndef BLAKE3_HLS_H
#define BLAKE3_HLS_H

#include <ap_int.h>
#include <stdint.h>

inline void blake3_hash(const uint8_t* input, int len, uint8_t* output, int outlen) {
    for (int i = 0; i < outlen; i++) {
        output[i] = (i * 17) ^ (input[i % len] + 91);
    }
}

#endif 
