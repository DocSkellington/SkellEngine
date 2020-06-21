#pragma once

#include "SkellEngine/random/RandomGenerator.hpp"

namespace engine::random {
    /**
     * \brief Combines the output of two random generators using the given function.
     * 
     * The function must have the following signature:
     * \code{.cpp}
     *  T (const T&, const T&)
     * \endcode
     * 
     * This generator can not be rigged (but the underlying generators can be).
     * 
     * \warning The function can not be a lambda.
     * 
     * \see MeanOfTwoRandomGenerators, SumOfTwoRandomGenerators, ProductOfTwoRandomGenerators, MaximumOfTwoRandomGenerators, and MinimumOfTwoRandomGenerators for already implemented combining functions
     */
    template <typename T, typename Func>
        requires std::is_invocable_r<T, Func, const T&, const T&>::value
    class CombinedTwoRandomGenerators : public RandomGenerator<T> {
    public:
        CombinedTwoRandomGenerators(RandomGenerator<T> *a, RandomGenerator<T> *b, const Func &function) :
            RandomGenerator<T>(false),
            m_a(a),
            m_b(b),
            m_function(function) {

        }

        /**
         * \brief Generates a value from each random generated and gives them to the combining function
         * \return The result of the combining function
         */
        T next() override {
            return std::invoke(m_function, m_a->next(), m_b->next());
        }

    private:
        RandomGenerator<T> *m_a;
        RandomGenerator<T> *m_b;
        // If we use a pointer, we can not accept lambdas.
        // If we do not use a pointer, we can not accept actual functions
        // I chose to accept actual functions (and to reject lambdas)
        Func* m_function;
    };

    namespace detail {
        template <typename T>
        T mean(const T& a, const T& b) {
            return (a + b) / 2;
        }

        template <typename T>
        T sum(const T& a, const T& b) {
            return a + b;
        }

        template <typename T>
        T product(const T& a, const T& b) {
            return a * b;
        }

        template <typename T>
        T max(const T& a, const T& b) {
            return std::max(a, b);
        }

        template <typename T>
        T min(const T& a, const T& b) {
            return std::min(a, b);
        }
    }

    /**
     * \brief Computes the mean of two random generators 
     */
    template <typename T>
    class MeanOfTwoRandomGenerators : public CombinedTwoRandomGenerators<T, decltype(detail::mean<T>)> {
    public:
        MeanOfTwoRandomGenerators(RandomGenerator<T> *a, RandomGenerator<T> *b) :
            CombinedTwoRandomGenerators<T, T(const T&, const T&)>(a, b, detail::mean<T>) {

        }
    };

    /**
     * \brief Computes the sum of two random generators 
     */
    template <typename T>
    class SumOfTwoRandomGenerators : public CombinedTwoRandomGenerators<T, decltype(detail::sum<T>)> {
    public:
        SumOfTwoRandomGenerators(RandomGenerator<T> *a, RandomGenerator<T> *b) :
            CombinedTwoRandomGenerators<T, T(const T&, const T&)>(a, b, detail::sum<T>) {

        }
    };

    /**
     * \brief Computes the product of two random generators 
     */
    template <typename T>
    class ProductOfTwoRandomGenerators : public CombinedTwoRandomGenerators<T, decltype(detail::product<T>)> {
    public:
        ProductOfTwoRandomGenerators(RandomGenerator<T> *a, RandomGenerator<T> *b) :
            CombinedTwoRandomGenerators<T, T(const T&, const T&)>(a, b, detail::sum<T>) {

        }
    };

    /**
     * \brief Computes the maximum of two random generators 
     */
    template <typename T>
    class MaximumOfTwoRandomGenerators : public CombinedTwoRandomGenerators<T, decltype(detail::max<T>)> {
    public:
        MaximumOfTwoRandomGenerators(RandomGenerator<T> *a, RandomGenerator<T> *b) :
            CombinedTwoRandomGenerators<T, T(const T&, const T&)>(a, b, detail::max<T>) {

        }
    };

    /**
     * \brief Computes the minimum of two random generators 
     */
    template <typename T>
    class MinimumOfTwoRandomGenerators : public CombinedTwoRandomGenerators<T, decltype(detail::min<T>)> {
    public:
        MinimumOfTwoRandomGenerators(RandomGenerator<T> *a, RandomGenerator<T> *b) :
            CombinedTwoRandomGenerators<T, T(const T&, const T&)>(a, b, detail::min<T>) {

        }
    };
}