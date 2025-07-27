#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <cstdint>
#include <random>


namespace adv_crypto {

/**
* BigInt - Arbitrary precision integer implementation for cryptographic applications
* 
* Features:
* - Arbitrary precision arithmetic
* - Constant-time operations where possible (for side-channel resistance)
* - Montgomery multiplication support
* - Modular arithmetic optimizations
* - Secure random number generation
* - Support for cryptographic operations (GCD, modular inverse, etc.)
*/
class BigInt {
    public:
    // Type definitions
    using digit_t = uint32_t;        // Base digit type
    using double_digit_t = uint64_t; // For intermediate calculations
    static constexpr int DIGIT_BITS = 32;
    static constexpr digit_t DIGIT_MAX = 0xFFFFFFFF;
    static constexpr double_digit_t DIGIT_BASE = 0x100000000ULL;

    private:
    std::vector<digit_t> digits_;  // Little-endian storage
    bool negative_;                // Sign flag

    // Internal helpers
    void normalize();
    void resize_to(size_t size);
    int compare_abs(const BigInt& other) const;

    // Low-level arithmetic helpers
    static digit_t add_with_carry(digit_t a, digit_t b, digit_t& carry);
    static digit_t sub_with_borrow(digit_t a, digit_t b, digit_t& borrow);
    static void multiply_digit(const std::vector<digit_t>& a, digit_t b, std::vector<digit_t>& result);
    static void divide_by_digit(const std::vector<digit_t>& dividend, digit_t divisor, 
                                std::vector<digit_t>& quotient, digit_t& remainder);

    public:
    // Constructors
    BigInt();
    BigInt(int64_t value);
    BigInt(const std::string& str, int base = 10);
    BigInt(const std::vector<digit_t>& digits, bool negative = false);
    BigInt(const BigInt& other) = default;
    BigInt(BigInt&& other) noexcept = default;
    
    // Assignment operators
    BigInt& operator=(const BigInt& other) = default;
    BigInt& operator=(BigInt&& other) noexcept = default;
    BigInt& operator=(int64_t value);
    
    // Comparison operators
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    
    // Arithmetic operators
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;
    BigInt operator%(const BigInt& other) const;
    
    BigInt& operator+=(const BigInt& other);
    BigInt& operator-=(const BigInt& other);
    BigInt& operator*=(const BigInt& other);
    BigInt& operator/=(const BigInt& other);
    BigInt& operator%=(const BigInt& other);
    
    // Unary operators
    BigInt operator-() const;
    BigInt operator+() const;
    
    // Increment/decrement
    BigInt& operator++();    // prefix
    BigInt operator++(int);  // postfix
    BigInt& operator--();    // prefix
    BigInt operator--(int);  // postfix
    
    // Bitwise operations
    BigInt operator<<(size_t shift) const;
    BigInt operator>>(size_t shift) const;
    BigInt& operator<<=(size_t shift);
    BigInt& operator>>=(size_t shift);
    
    BigInt operator&(const BigInt& other) const;
    BigInt operator|(const BigInt& other) const;
    BigInt operator^(const BigInt& other) const;
    BigInt operator~() const;
    
    // Utility methods
    bool is_zero() const;
    bool is_one() const;
    bool is_negative() const;
    bool is_even() const;
    bool is_odd() const;
    
    size_t bit_length() const;
    size_t byte_length() const;
    bool get_bit(size_t index) const;
    void set_bit(size_t index, bool value = true);
    void clear_bit(size_t index);
    
    BigInt abs() const;
    void negate();
    
    // String representation
    std::string to_string(int base = 10) const;
    std::string to_hex_string() const;
    std::vector<uint8_t> to_bytes() const;
    static BigInt from_bytes(const std::vector<uint8_t>& bytes);
    
    // Cryptographic operations
    static BigInt gcd(const BigInt& a, const BigInt& b);
    static BigInt extended_gcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y);
    BigInt mod_inverse(const BigInt& modulus) const;
    BigInt mod_pow(const BigInt& exponent, const BigInt& modulus) const;
    
    
    // Constant-time operations (for side-channel resistance)
    static BigInt ct_select(bool condition, const BigInt& a, const BigInt& b);
    static void ct_swap(bool condition, BigInt& a, BigInt& b);
    
    // Random number generation
    static BigInt random_bits(size_t bits, std::mt19937& rng);
    static BigInt random_range(const BigInt& min, const BigInt& max, std::mt19937& rng);
    static BigInt random_prime(size_t bits, std::mt19937& rng, int certainty = 40);
    
    // Primality testing
    bool is_prime(int certainty = 40) const;
    bool miller_rabin_test(const BigInt& witness) const;
    
    // Special mathematical functions
    BigInt sqrt() const;
    bool is_perfect_square() const;
    static BigInt lcm(const BigInt& a, const BigInt& b);
    
    // Serialization helpers
    void serialize(std::ostream& os) const;
    static BigInt deserialize(std::istream& is);
    
    // Friend functions for I/O
    friend std::ostream& operator<<(std::ostream& os, const BigInt& bi);
    friend std::istream& operator>>(std::istream& is, BigInt& bi);
    
    // Debug and testing helpers
    void print_debug() const;
    bool self_test() const;
    
    // Access to internal representation (use with caution)
    const std::vector<digit_t>& get_digits() const { return digits_; }
    size_t size() const { return digits_.size(); }
};

// Global operators
BigInt operator+(int64_t lhs, const BigInt& rhs);
BigInt operator-(int64_t lhs, const BigInt& rhs);
BigInt operator*(int64_t lhs, const BigInt& rhs);
BigInt operator/(int64_t lhs, const BigInt& rhs);
BigInt operator%(int64_t lhs, const BigInt& rhs);

// Utility functions
BigInt pow(const BigInt& base, const BigInt& exponent);
BigInt factorial(const BigInt& n);

// Constants
extern const BigInt BIGINT_ZERO;
extern const BigInt BIGINT_ONE;
extern const BigInt BIGINT_TWO;

}