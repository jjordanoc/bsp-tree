#ifndef DATATYPE_H
#define DATATYPE_H

#include <cmath>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <iostream>

template <typename T>
class Safe {
    // Only allows 'float', 'double', or 'long double'
    static_assert(std::is_floating_point<T>::value, "Template type must be a floating-point type");

private:
    T value;
    static constexpr T EPSILON = static_cast<T>(1e-6);

public:
    // Constructors
    Safe() : value(static_cast<T>(0)) {}
    Safe(T val) : value(val) {}

    // Accessors
    inline T getValue() const { return value; }
    inline void setValue(T val) { value = val; }

    // Unary operators
    Safe operator-() const { return Safe(-value); }

    // Arithmetic operators with Safe<T>
    Safe& operator+=(const Safe& other) {
        value += other.value;
        return *this;
    }
    Safe& operator-=(const Safe& other) {
        value -= other.value;
        return *this;
    }
    Safe& operator*=(const Safe& other) {
        value *= other.value;
        return *this;
    }
    Safe& operator/=(const Safe& other) {
        if (std::abs(other.value) < EPSILON) {
            throw std::runtime_error("Division by zero");
        }
        value /= other.value;
        return *this;
    }

    // Arithmetic operators with built-in types
    template <typename U>
    Safe& operator+=(const U& other) {
        static_assert(std::is_arithmetic<U>::value, "Operation only valid with numeric types");
        value += static_cast<T>(other);
        return *this;
    }
    template <typename U>
    Safe& operator-=(const U& other) {
        static_assert(std::is_arithmetic<U>::value, "Operation only valid with numeric types");
        value -= static_cast<T>(other);
        return *this;
    }
    template <typename U>
    Safe& operator*=(const U& other) {
        static_assert(std::is_arithmetic<U>::value, "Operation only valid with numeric types");
        value *= static_cast<T>(other);
        return *this;
    }
    template <typename U>
    Safe& operator/=(const U& other) {
        static_assert(std::is_arithmetic<U>::value, "Operation only valid with numeric types");
        if (std::abs(static_cast<T>(other)) < EPSILON) {
            throw std::runtime_error("Division by zero");
        }
        value /= static_cast<T>(other);
        return *this;
    }

    // Arithmetic operators
    friend Safe operator+(Safe lhs, const Safe& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend Safe operator-(Safe lhs, const Safe& rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend Safe operator*(Safe lhs, const Safe& rhs) {
        lhs *= rhs;
        return lhs;
    }
    friend Safe operator/(Safe lhs, const Safe& rhs) {
        lhs /= rhs;
        return lhs;
    }

    // Arithmetic operators with built-in types
    template <typename U>
    friend Safe operator+(Safe lhs, const U& rhs) {
        lhs += rhs;
        return lhs;
    }
    template <typename U>
    friend Safe operator-(Safe lhs, const U& rhs) {
        lhs -= rhs;
        return lhs;
    }
    template <typename U>
    friend Safe operator*(Safe lhs, const U& rhs) {
        lhs *= rhs;
        return lhs;
    }
    template <typename U>
    friend Safe operator/(Safe lhs, const U& rhs) {
        lhs /= rhs;
        return lhs;
    }

    // Comparison operators with Safe<T>
    bool operator==(const Safe& other) const {
        return std::abs(value - other.value) < EPSILON;
    }
    bool operator!=(const Safe& other) const { return !(*this == other); }
    bool operator<(const Safe& other) const {
        return value < other.value - EPSILON;
    }
    bool operator<=(const Safe& other) const {
        return value <= other.value + EPSILON;
    }
    bool operator>(const Safe& other) const {
        return value > other.value + EPSILON;
    }
    bool operator>=(const Safe& other) const {
        return value >= other.value - EPSILON;
    }

    // Comparison operators with built-in types
    template <typename U>
    bool operator==(const U& other) const {
        static_assert(std::is_arithmetic<U>::value, "Comparison only valid with numeric types");
        return std::abs(value - static_cast<T>(other)) < EPSILON;
    }
    template <typename U>
    bool operator!=(const U& other) const { return !(*this == other); }
    template <typename U>
    bool operator<(const U& other) const {
        return value < static_cast<T>(other) - EPSILON;
    }
    template <typename U>
    bool operator<=(const U& other) const {
        return value <= static_cast<T>(other) + EPSILON;
    }
    template <typename U>
    bool operator>(const U& other) const {
        return value > static_cast<T>(other) + EPSILON;
    }
    template <typename U>
    bool operator>=(const U& other) const {
        return value >= static_cast<T>(other) - EPSILON;
    }

    // Friend functions for symmetric comparison with built-in types
    template <typename U>
    friend bool operator==(const U& lhs, const Safe& rhs) {
        return rhs == lhs;
    }
    template <typename U>
    friend bool operator!=(const U& lhs, const Safe& rhs) {
        return rhs != lhs;
    }
    template <typename U>
    friend bool operator<(const U& lhs, const Safe& rhs) {
        return lhs < rhs.value - EPSILON;
    }
    template <typename U>
    friend bool operator<=(const U& lhs, const Safe& rhs) {
        return lhs <= rhs.value + EPSILON;
    }
    template <typename U>
    friend bool operator>(const U& lhs, const Safe& rhs) {
        return lhs > rhs.value + EPSILON;
    }
    template <typename U>
    friend bool operator>=(const U& lhs, const Safe& rhs) {
        return lhs >= rhs.value - EPSILON;
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Safe& obj) {
        os << obj.value;
        return os;
    }

    // Mathematical functions
    friend Safe abs(const Safe& x) {
        return Safe(std::abs(x.value));
    }
    friend Safe sqrt(const Safe& x) {
        if (x.value < static_cast<T>(0)) {
            throw std::runtime_error("Attempted to calculate the square root of a negative number");
        }
        return Safe(std::sqrt(x.value));
    }
    friend Safe pow(const Safe& base, const T& exponent) {
        return Safe(std::pow(base.value, exponent));
    }
    friend Safe min(const Safe& a, const Safe& b) {
        return a < b ? a : b;
    }
    friend Safe max(const Safe& a, const Safe& b) {
        return a > b ? a : b;
    }

    // Trigonometric functions
    friend Safe sin(const Safe& x) {
        return Safe(std::sin(x.value));
    }
    friend Safe cos(const Safe& x) {
        return Safe(std::cos(x.value));
    }
    friend Safe tan(const Safe& x) {
        return Safe(std::tan(x.value));
    }
    friend Safe asin(const Safe& x) {
        if (x.value < static_cast<T>(-1) || x.value > static_cast<T>(1)) {
            throw std::runtime_error("Invalid input for arcsin");
        }
        return Safe(std::asin(x.value));
    }
    friend Safe acos(const Safe& x) {
        if (x.value < static_cast<T>(-1) || x.value > static_cast<T>(1)) {
            throw std::runtime_error("Invalid input for arccos");
        }
        return Safe(std::acos(x.value));
    }
    friend Safe atan(const Safe& x) {
        return Safe(std::atan(x.value));
    }

    // Additional mathematical functions
    friend Safe exp(const Safe& x) {
        return Safe(std::exp(x.value));
    }
    friend Safe log(const Safe& x) {
        if (x.value <= static_cast<T>(0)) {
            throw std::runtime_error("Attempted to calculate the logarithm of a non-positive number");
        }
        return Safe(std::log(x.value));
    }
};

template <typename T>
Safe<T> sqrt(const Safe<T>& x) {
    return Safe<T>::sqrt(x);
}
template <typename T>
Safe<T> sin(const Safe<T>& x) {
    return Safe<T>::sin(x);
}
template <typename T>
Safe<T> cos(const Safe<T>& x) {
    return Safe<T>::cos(x);
}
template <typename T>
Safe<T> tan(const Safe<T>& x) {
    return Safe<T>::tan(x);
}
template <typename T>
Safe<T> asin(const Safe<T>& x) {
    return Safe<T>::asin(x);
}
template <typename T>
Safe<T> acos(const Safe<T>& x) {
    return Safe<T>::acos(x);
}
template <typename T>
Safe<T> atan(const Safe<T>& x) {
    return Safe<T>::atan(x);
}
template <typename T>
Safe<T> exp(const Safe<T>& x) {
    return Safe<T>::exp(x);
}
template <typename T>
Safe<T> log(const Safe<T>& x) {
    return Safe<T>::log(x);
}
template <typename T>
Safe<T> pow(const Safe<T>& base, const T& exponent) {
    return Safe<T>::pow(base, exponent);
}

// Typedefs
using NType = Safe<double>;

// Relation type
enum RelationType {
    COINCIDENT,
    IN_FRONT,
    BEHIND,
    SPLIT
};

// Overload operator<< for RelationType
inline std::ostream& operator<<(std::ostream& os, const RelationType& type) {
    switch (type) {
        case COINCIDENT: os << "COINCIDENT"; break;
        case IN_FRONT: os << "IN_FRONT"; break;
        case BEHIND: os << "BEHIND"; break;
        case SPLIT: os << "SPLIT"; break;
        default: os << "UNKNOWN"; break;
    }
    return os;
}

#endif // DATATYPE_H
