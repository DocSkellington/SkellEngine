#pragma once

#include <ostream>
#include <istream>
#include <compare>

#include <SFML/System/Vector2.hpp>

#include "SkellEngine/Concepts.hpp"

namespace engine{
    /**
     * \brief A vector in a two-dimensional space.
     * 
     * The "cross product" is not defined as it not meaningful in 2D.
     */
    template<typename T>
    class Vector2 {
    public:
        T x;
        T y;
    
    public:
        Vector2() requires std::is_constructible_v<T, void> || std::is_fundamental_v<T> :
            x(T()),
            y(T()) {}

        Vector2(T x, T y) :
            x(x),
            y(y) {}

        template <typename B>
            requires std::is_convertible_v<B, T> && ThreeWayComparable<T>
        auto operator<=>(const Vector2<B> &rhs) const {
            if (auto cmp = x <=> rhs.x ; cmp != 0) {
                return cmp;
            }
            return y <=> rhs.y;
        }

        template <typename B>
            requires std::is_convertible_v<B, T> && EqualToable<B, T>
        bool operator==(const Vector2<B> &rhs) const {
            // For some reasons, the compiler complains that == is not defined, even though <=> is
            // So, we explicitly add this overload
            return x == rhs.x && y == rhs.y;
        }

        Vector2<T> operator+(const Vector2<T> &other) const requires Addable<T> {
            return Vector2<T>(x + other.x, y + other.y);
        }

        Vector2<T> operator+(const T &num) const requires Addable<T> {
            return Vector2<T>(x + num, y + num);
        }

        Vector2<T> operator+=(const Vector2<T> &other) const requires Addable<T> {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2<T> operator+=(const T &num) requires AddableAssignable<T> {
            x += num;
            y += num;
            return *this;
        }

        T operator*(const Vector2<T> &other) const requires Multipliable<T> && Multipliable<T> {
            return dot(other);
        }

        Vector2<T> operator*(const T &num) const requires Multipliable<T> {
            return Vector2<T>(x * num, y * num);
        }

        Vector2<T> operator*=(const T &num) requires MultipliableAssignable<T> {
            x *= num;
            y *= num;
            return *this;
        }

        T dot(const Vector2<T> &other) const requires Addable<T> && Multipliable<T> {
            return x * other.x + y * other.y;
        }

        operator sf::Vector2<T>() const {
            return sf::Vector2<T>(x, y);
        }

        operator sf::Vector2f() const requires std::is_convertible_v<T, float> {
            return sf::Vector2f((float) x, (float) y);
        }
    };

    template<typename T>
        requires OutputStreamable<T>
    std::ostream& operator<<(std::ostream &os, const Vector2<T> &vec) {
        os << '(' << vec.x << ", " << vec.y << ')';
        return os;
    }

    template<typename T>
        requires InputStreamable<T>
    std::istream& operator>>(std::istream &is, Vector2<T> &vec) {
        is >> vec.x >> vec.y;
        return is;
    }

    /**
     * \brief A 2D vector using char
     */
    using Vector2c = Vector2<char>;
    /**
     * \brief A 2D vector using unsigned char
     */
    using Vector2uc = Vector2<unsigned char>;
    /**
     * \brief A 2D vector using int
     */
    using Vector2i = Vector2<int>;
    /**
     * \brief A 2D vector using unsigned int
     */
    using Vector2ui = Vector2<unsigned int>;
    /**
     * \brief A 2D vector using long
     */
    using Vector2l = Vector2<long>;
    /**
     * \brief A 2D vector using unsigned long
     */
    using Vector2ul = Vector2<unsigned long>;
    /**
     * \brief A 2D vector using float
     */
    using Vector2f = Vector2<float>;
    /**
     * \brief A 2D vector using double
     */
    using Vector2d = Vector2<double>;
}
