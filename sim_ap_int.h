#ifndef SIM_AP_INT_H
#define SIM_AP_INT_H

#include <stdint.h>
#include <cstring>

template <int W>
struct ap_uint {
    static const int WORDS = (W + 63) / 64;
    uint64_t val[WORDS];

    ap_uint(uint64_t init = 0) {
        memset(val, 0, sizeof(val));
        val[0] = init;
    }

    operator uint64_t() const { return val[0]; }

    ap_uint<W>& operator=(uint64_t v) {
        memset(val, 0, sizeof(val));
        val[0] = v;
        return *this;
    }

    // Simulate basic range write
    void set_range(int hi, int lo, uint64_t v) {
        int word_lo = lo / 64;
        int word_hi = hi / 64;
        int bit_lo = lo % 64;
        int bit_len = hi - lo + 1;

        uint64_t mask = ((1ULL << bit_len) - 1);
        val[word_lo] &= ~(mask << bit_lo);
        val[word_lo] |= (v & mask) << bit_lo;
    }
    ap_uint(const ap_uint<W>& other) {
        for (int i = 0; i < WORDS; i++) {
            val[i] = other.val[i];
        }
    }

    // Conversion from volatile ap_uint<W>
    ap_uint(const volatile ap_uint<W>& other) {
        for (int i = 0; i < WORDS; i++) {
            val[i] = other.val[i];
        }
    }

    // Simulate basic range read
    uint64_t range(int hi, int lo) const {
        int word_lo = lo / 64;
        int bit_lo = lo % 64;
        int bit_len = hi - lo + 1;

        uint64_t mask = ((1ULL << bit_len) - 1);
        return (val[word_lo] >> bit_lo) & mask;
    }

    ap_uint<W>& operator^=(const ap_uint<W>& rhs) {
        for (int i = 0; i < WORDS; i++) val[i] ^= rhs.val[i];
        return *this;
    }

    ap_uint<W>& operator+=(const ap_uint<W>& rhs) {
        for (int i = 0; i < WORDS; i++) val[i] += rhs.val[i];  // very naive
        return *this;
    }
};

#endif
