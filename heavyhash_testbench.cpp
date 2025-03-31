#ifndef __SYNTHESIS__
#include "sim_ap_int.h"
#else
#include <ap_int.h>
#endif

#include <iostream>
#include <iomanip>
#include "heavyhash_hls.cpp"


int main() {
    hash256_t input_hash = 0;
    input_hash.set_range(255, 192, 0x81553a695a058899);
    input_hash.set_range(191, 128, 0x8c413792e74ce8b8);
    input_hash.set_range(127, 64,  0xf8a096d64b3ee473);
    input_hash.set_range(63, 0,    0x87372434485c0b6f);
    uint64_t timestamp = 0x000001848ca87c49;
    uint64_t nonce = 0x2f8400000eba167c;
    u8 digest[32];

    heavyhash_core(input_hash, timestamp, nonce, digest);

    std::cout << "Digest = ";
    for (int i = 0; i < 32; i++)
        std::printf("%02x", digest[i]);
    std::cout << std::endl;

    // TODO: Add comparison with expected result
    // Expected: 000000001726686e851f02c584d7cc8a8fbe5938ecdb3ffa2ba16c260ee1fc40

    return 0;
}
