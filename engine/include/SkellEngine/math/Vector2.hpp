#pragma once

#include <ostream>
#include <compare>

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
        Vector2(T x, T y) :
            x(x),
            y(y) {}

        auto operator<=>(const Vector2<T> &other) const = default;

        Vector2<T> operator+(const Vector2<T> &other) const {
            return Vector2<T>(x + other.x, y + other.y);
        }

        Vector2<T> operator+(const T &num) const {
            return Vector2<T>(x + num, y + num);
        }

        Vector2<T> operator+=(const Vector2<T> &other) const {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2<T> operator+=(const T &num) {
            x += num;
            y += num;
            return *this;
        }

        T operator*(const Vector2<T> &other) const {
            return dot(other);
        }

        Vector2<T> operator*(const T &num) const {
            return Vector2<T>(x * num, y * num);
        }

        Vector2<T> operator*=(const T &num) {
            x *= num;
            y *= num;
            return *this;
        }

        T dot(const Vector2<T> &other) const {
            return x * other.x + y * other.y;
        }
    };

    template<typename T>
    std::ostream& operator<<(std::ostream &os, const Vector2<T> &vec) {
        os << '(' << vec.x << ", " << vec.y << ')';
        return os;
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
