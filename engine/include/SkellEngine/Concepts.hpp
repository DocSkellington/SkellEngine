/**
 * \file Concepts.hpp
 * 
 * This file defines a few "concepts" used to determine if the addition, the multiplication, and so on are defined
 */

#include <concepts>
#include <ostream>
#include <istream>

/**
 * \brief Requires T and S to be addable.
 * 
 * That is, the operator+ must be defined for two variables T and S.
 * \warning The order is important if the addition is not commutable.
 */
template <typename T, typename S = T>
concept Addable = requires(T a, S b) {
    a + b;
};

/**
 * \brief Requires T += S to be defined.
 * 
 * That is, the operator+= must be defined for two variables T and S.
 * \warning The order is important.
 */
template <typename T, typename S = T>
concept AddableAssignable = requires(T& a, S b) {
    a += b;
};

/**
 * \brief Requires T and S to be subtractable.
 * 
 * That is, the operator- must be defined for two variables T and S.
 * \warning The order is important if the substraction is not commutable.
 */
template <typename T, typename S = T>
concept Substractable = requires(T a, S b) {
    a - b;
};

/**
 * \brief Requires T -= S to be defined.
 * 
 * That is, the operator-= must be defined for two variables T and S.
 * \warning The order is important.
 */
template <typename T, typename S = T>
concept SubstractableAssignable = requires(T& a, S b) {
    a -= b;
};

/**
 * \brief Requires T and S to be multipliable
 * 
 * That is, the operator* must be defined for two variables T and S.
 * \warning The order is important if the addition is not commutable.
 */
template <typename T, typename S = T>
concept Multipliable = requires(T a, S b) {
    a * b;
};

/**
 * \brief Requires T *= S to be defined.
 * 
 * That is, the operator*= must be defined for two variables T and S.
 * \warning The order is important.
 */
template <typename T, typename S = T>
concept MultipliableAssignable = requires(T& a, S b) {
    a *= b;
};

/**
 * \brief Requires T and S to be divisible
 * 
 * That is, the operator/ must be defined for two variables T and S.
 * \warning The order is important if the division is not commutable.
 */
template <typename T, typename S = T>
concept Divisible = requires(T a, S b) {
    a / b;
};

/**
 * \brief Requires T /= S to be defined.
 * 
 * That is, the operator/= must be defined for two variables T and S.
 * \warning The order is important.
 */
template <typename T, typename S = T>
concept DivisibleAssignable = requires(T& a, S b) {
    a /= b;
};

/**
 * \brief Requires T == S to be defined.
 * \warning The order is important.
 */
template <typename T, typename S = T>
concept EqualToable = requires(T a, S b) {
    a == b;
};

/**
 * \brief Requires T != S to be defined.
 * \warning The order is important.
 */
template <typename T, typename S = T>
concept NotEqualToable = requires(T a, S b) {
    a != b;
};

/**
 * \brief Requires T > S to be defined.
 * \warning The order is important.
 */
template <typename T, typename S = T>
concept GreaterThanable = requires(T a, S b) {
    a > b;
};

/**
 * \brief Requires T < S to be defined.
 * \warning The order is important.
 */
template <typename T, typename S = T>
concept LowerThanable = requires(T a, S b) {
    a < b;
};

/**
 * \brief Requires T <=> S to be defined.
 * \warning The order is important.
 */
template <typename T, typename S = T>
concept ThreeWayComparable = requires (T a, S b) {
    a <=> b;
};

/**
 * \brief Requires T to define the operator<< for an output stream.
 */
template <typename T>
concept OutputStreamable = requires(std::ostream &os, T a) {
    os << a;
};

/**
 * \brief Requires T to define the operator>> for an input stream.
 */
template <typename T>
concept InputStreamable = requires(std::istream &is, T a) {
    is >> a;
};