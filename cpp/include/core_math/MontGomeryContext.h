#pragma once

#include "BigInt.h"

// Montgomery arithmetic support
class MontgomeryContext {
    private:
        BigInt modulus_;
        BigInt r_;           // R = 2^k where k >= bit_length(modulus)
        BigInt r_inv_;       // R^-1 mod modulus
        BigInt n_prime_;     // -modulus^-1 mod R
        size_t k_;           // Number of bits in R
        
    public:
        MontgomeryContext(const BigInt& modulus);
        BigInt to_montgomery(const BigInt& x) const;
        BigInt from_montgomery(const BigInt& x) const;
        BigInt montgomery_multiply(const BigInt& a, const BigInt& b) const;
        BigInt montgomery_square(const BigInt& a) const;
        const BigInt& get_modulus() const { return modulus_; }
    };