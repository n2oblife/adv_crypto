#include "core_math/BigInt.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <iomanip>
#include <sstream>

// Constants
const BigInt BIGINT_ZERO(0);
const BigInt BIGINT_ONE(1);
const BigInt BIGINT_TWO(2);

// Constructors
BigInt::BigInt() : negative_(false) {
    digits_.push_back(0);
}

BigInt::BigInt(int64_t value) : negative_(value < 0) {
    uint64_t abs_value = negative_ ? -static_cast<uint64_t>(value) : static_cast<uint64_t>(value);
    
    if (abs_value == 0) {
        digits_.push_back(0);
        negative_ = false;
    } else {
        while (abs_value > 0) {
            digits_.push_back(static_cast<digit_t>(abs_value & DIGIT_MAX));
            abs_value >>= DIGIT_BITS;
        }
    }
}

BigInt::BigInt(const std::string& str, int base) : negative_(false) {
    if (str.empty()) {
        digits_.push_back(0);
        return;
    }
    
    size_t start = 0;
    if (str[0] == '-') {
        negative_ = true;
        start = 1;
    } else if (str[0] == '+') {
        start = 1;
    }
    
    // Handle hex prefix
    if (base == 16 && start + 1 < str.length() && 
        str[start] == '0' && (str[start + 1] == 'x' || str[start + 1] == 'X')) {
        start += 2;
    }
    
    *this = BigInt(0);
    BigInt base_power(1);
    BigInt base_bi(base);
    
    // Process digits from right to left
    for (int i = str.length() - 1; i >= static_cast<int>(start); --i) {
        char c = str[i];
        int digit_value;
        
        if (c >= '0' && c <= '9') {
            digit_value = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit_value = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z') {
            digit_value = c - 'a' + 10;
        } else {
            throw std::invalid_argument("Invalid character in number string");
        }
        
        if (digit_value >= base) {
            throw std::invalid_argument("Digit value exceeds base");
        }
        
        *this += BigInt(digit_value) * base_power;
        base_power *= base_bi;
    }
    
    if (is_zero()) {
        negative_ = false;
    }
}

BigInt::BigInt(const std::vector<digit_t>& digits, bool negative) 
    : digits_(digits), negative_(negative) {
    normalize();
}

// Assignment
BigInt& BigInt::operator=(int64_t value) {
    *this = BigInt(value);
    return *this;
}

// Utility methods
void BigInt::normalize() {
    // Remove leading zeros
    while (digits_.size() > 1 && digits_.back() == 0) {
        digits_.pop_back();
    }
    
    // Handle zero case
    if (digits_.size() == 1 && digits_[0] == 0) {
        negative_ = false;
    }
}

void BigInt::resize_to(size_t size) {
    if (size > digits_.size()) {
        digits_.resize(size, 0);
    }
}

bool BigInt::is_zero() const {
    return digits_.size() == 1 && digits_[0] == 0;
}

bool BigInt::is_one() const {
    return !negative_ && digits_.size() == 1 && digits_[0] == 1;
}

bool BigInt::is_negative() const {
    return negative_ && !is_zero();
}

bool BigInt::is_even() const {
    return (digits_[0] & 1) == 0;
}

bool BigInt::is_odd() const {
    return (digits_[0] & 1) == 1;
}

size_t BigInt::bit_length() const {
    if (is_zero()) return 1;
    
    size_t bits = (digits_.size() - 1) * DIGIT_BITS;
    digit_t top_digit = digits_.back();
    
    while (top_digit > 0) {
        bits++;
        top_digit >>= 1;
    }
    
    return bits;
}

size_t BigInt::byte_length() const {
    return (bit_length() + 7) / 8;
}

bool BigInt::get_bit(size_t index) const {
    size_t digit_index = index / DIGIT_BITS;
    size_t bit_index = index % DIGIT_BITS;
    
    if (digit_index >= digits_.size()) {
        return false;
    }
    
    return (digits_[digit_index] >> bit_index) & 1;
}

void BigInt::set_bit(size_t index, bool value) {
    size_t digit_index = index / DIGIT_BITS;
    size_t bit_index = index % DIGIT_BITS;
    
    if (digit_index >= digits_.size()) {
        if (value) {
            digits_.resize(digit_index + 1, 0);
        } else {
            return; // Bit is already 0
        }
    }
    
    if (value) {
        digits_[digit_index] |= (1U << bit_index);
    } else {
        digits_[digit_index] &= ~(1U << bit_index);
    }
    
    if (!value) {
        normalize();
    }
}

void BigInt::clear_bit(size_t index) {
    set_bit(index, false);
}

BigInt BigInt::abs() const {
    BigInt result(*this);
    result.negative_ = false;
    return result;
}

void BigInt::negate() {
    if (!is_zero()) {
        negative_ = !negative_;
    }
}

// Comparison
int BigInt::compare_abs(const BigInt& other) const {
    if (digits_.size() < other.digits_.size()) return -1;
    if (digits_.size() > other.digits_.size()) return 1;
    
    for (int i = digits_.size() - 1; i >= 0; --i) {
        if (digits_[i] < other.digits_[i]) return -1;
        if (digits_[i] > other.digits_[i]) return 1;
    }
    
    return 0;
}

bool BigInt::operator==(const BigInt& other) const {
    return negative_ == other.negative_ && digits_ == other.digits_;
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const {
    if (negative_ != other.negative_) {
        return negative_;
    }
    
    int cmp = compare_abs(other);
    return negative_ ? (cmp > 0) : (cmp < 0);
}

bool BigInt::operator<=(const BigInt& other) const {
    return *this < other || *this == other;
}

bool BigInt::operator>(const BigInt& other) const {
    return !(*this <= other);
}

bool BigInt::operator>=(const BigInt& other) const {
    return !(*this < other);
}

// Arithmetic helpers
BigInt::digit_t BigInt::add_with_carry(digit_t a, digit_t b, digit_t& carry) {
    double_digit_t sum = static_cast<double_digit_t>(a) + b + carry;
    carry = static_cast<digit_t>(sum >> DIGIT_BITS);
    return static_cast<digit_t>(sum & DIGIT_MAX);
}

BigInt::digit_t BigInt::sub_with_borrow(digit_t a, digit_t b, digit_t& borrow) {
    double_digit_t diff = static_cast<double_digit_t>(a) - b - borrow;
    if (diff & (1ULL << 63)) { // Check if negative
        borrow = 1;
        diff += DIGIT_BASE;
    } else {
        borrow = 0;
    }
    return static_cast<digit_t>(diff & DIGIT_MAX);
}

// Arithmetic operations
BigInt BigInt::operator+(const BigInt& other) const {
    if (negative_ == other.negative_) {
        // Same sign: add magnitudes
        BigInt result;
        result.negative_ = negative_;
        
        size_t max_size = std::max(digits_.size(), other.digits_.size());
        result.digits_.resize(max_size + 1, 0);
        
        digit_t carry = 0;
        for (size_t i = 0; i < max_size; ++i) {
            digit_t a = (i < digits_.size()) ? digits_[i] : 0;
            digit_t b = (i < other.digits_.size()) ? other.digits_[i] : 0;
            result.digits_[i] = add_with_carry(a, b, carry);
        }
        
        if (carry) {
            result.digits_[max_size] = carry;
        }
        
        result.normalize();
        return result;
    } else {
        // Different signs: subtract magnitudes
        if (compare_abs(other) >= 0) {
            return *this - (-other);
        } else {
            return other - (-*this);
        }
    }
}

BigInt BigInt::operator-(const BigInt& other) const {
    if (negative_ != other.negative_) {
        // Different signs: add magnitudes
        return *this + (-other);
    }
    
    // Same sign: subtract magnitudes
    int cmp = compare_abs(other);
    if (cmp == 0) {
        return BigInt(0);
    }
    
    BigInt result;
    if (cmp > 0) {
        // |this| > |other|
        result.negative_ = negative_;
        result.digits_.resize(digits_.size(), 0);
        
        digit_t borrow = 0;
        for (size_t i = 0; i < digits_.size(); ++i) {
            digit_t b = (i < other.digits_.size()) ? other.digits_[i] : 0;
            result.digits_[i] = sub_with_borrow(digits_[i], b, borrow);
        }
    } else {
        // |this| < |other|
        result.negative_ = !negative_;
        result.digits_.resize(other.digits_.size(), 0);
        
        digit_t borrow = 0;
        for (size_t i = 0; i < other.digits_.size(); ++i) {
            digit_t a = (i < digits_.size()) ? digits_[i] : 0;
            result.digits_[i] = sub_with_borrow(other.digits_[i], a, borrow);
        }
    }
    
    result.normalize();
    return result;
}

BigInt BigInt::operator*(const BigInt& other) const {
    if (is_zero() || other.is_zero()) {
        return BigInt(0);
    }
    
    BigInt result;
    result.negative_ = negative_ != other.negative_;
    result.digits_.resize(digits_.size() + other.digits_.size(), 0);
    
    // Grade school multiplication
    for (size_t i = 0; i < digits_.size(); ++i) {
        digit_t carry = 0;
        for (size_t j = 0; j < other.digits_.size(); ++j) {
            double_digit_t product = static_cast<double_digit_t>(digits_[i]) * other.digits_[j];
            product += result.digits_[i + j] + carry;
            
            result.digits_[i + j] = static_cast<digit_t>(product & DIGIT_MAX);
            carry = static_cast<digit_t>(product >> DIGIT_BITS);
        }
        
        if (carry) {
            result.digits_[i + other.digits_.size()] += carry;
        }
    }
    
    result.normalize();
    return result;
}

// Compound assignment operators
BigInt& BigInt::operator+=(const BigInt& other) {
    *this = *this + other;
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& other) {
    *this = *this - other;
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& other) {
    *this = *this * other;
    return *this;
}

// Unary operators
BigInt BigInt::operator-() const {
    BigInt result(*this);
    result.negate();
    return result;
}

BigInt BigInt::operator+() const {
    return *this;
}

// String conversion
std::string BigInt::to_string(int base) const {
    if (base < 2 || base > 36) {
        throw std::invalid_argument("Base must be between 2 and 36");
    }
    
    if (is_zero()) {
        return "0";
    }
    
    std::string result;
    BigInt temp = abs();
    BigInt base_bi(base);
    
    while (!temp.is_zero()) {
        BigInt remainder = temp % base_bi;
        temp = temp / base_bi;
        
        int digit = static_cast<int>(remainder.digits_[0]);
        if (digit < 10) {
            result = char('0' + digit) + result;
        } else {
            result = char('A' + digit - 10) + result;
        }
    }
    
    if (negative_) {
        result = "-" + result;
    }
    
    return result;
}

std::string BigInt::to_hex_string() const {
    if (is_zero()) {
        return "0x0";
    }
    
    std::stringstream ss;
    ss << (negative_ ? "-0x" : "0x");
    
    bool first = true;
    for (int i = digits_.size() - 1; i >= 0; --i) {
        if (first && digits_[i] == 0) continue;
        
        if (first) {
            ss << std::hex << digits_[i];
            first = false;
        } else {
            ss << std::setfill('0') << std::setw(8) << std::hex << digits_[i];
        }
    }
    
    return ss.str();
}

// I/O operators
std::ostream& operator<<(std::ostream& os, const BigInt& bi) {
    return os << bi.to_string();
}

// Debug helper
void BigInt::print_debug() const {
    std::cout << "BigInt: " << (negative_ ? "-" : "+") << " [";
    for (size_t i = 0; i < digits_.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << std::hex << digits_[i];
    }
    std::cout << "] (" << digits_.size() << " digits)" << std::endl;
}

// Basic self-test
bool BigInt::self_test() const {
    // Test basic properties
    BigInt zero(0);
    BigInt one(1);
    BigInt two(2);
    
    // Test zero properties
    if (!zero.is_zero() || zero.is_negative()) return false;
    if (zero + one != one) return false;
    if (zero * one != zero) return false;
    
    // Test one properties
    if (!one.is_one() || one.is_zero()) return false;
    if (one + one != two) return false;
    if (one * two != two) return false;
    
    // Test comparison
    if (!(zero < one) || !(one > zero)) return false;
    if (zero == one || !(zero != one)) return false;
    
    return true;
}