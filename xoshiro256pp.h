#ifndef XOSHIRO256PP_H
#define XOSHIRO256PP_H

#include <ap_int.h>

class Xoshiro256PP {
public:
    ap_uint<64> s[4];

    void init(ap_uint<64> s0, ap_uint<64> s1, ap_uint<64> s2, ap_uint<64> s3) {
        s[0] = s0; s[1] = s1; s[2] = s2; s[3] = s3;
    }

    ap_uint<64> rotl(ap_uint<64> x, int k) {
        return (x << k) | (x >> (64 - k));
    }

    ap_uint<64> next() {
#pragma HLS INLINE
        ap_uint<64> result = rotl(s[0] + s[3], 23) + s[0];

        ap_uint<64> t = s[1] << 17;
        s[2] ^= s[0]; s[3] ^= s[1]; s[1] ^= s[2]; s[0] ^= s[3];
        s[2] ^= t;
        s[3] = rotl(s[3], 45);

        return result;
    }
};

#endif

