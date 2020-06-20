#pragma once

#include "SkellEngine/random/RandomGenerator.hpp"

namespace engine::random {
    template <typename T, typename Func>
        requires std::is_invocable_r<T, Func, const T&, const T&>::value
    class CombinedRandomGenerator : public RandomGenerator<T> {
    public:
        CombinedRandomGenerator(RandomGenerator<T> *a, RandomGenerator<T> *b, const Func &function) :
            RandomGenerator<T>(false),
            m_a(a),
            m_b(b),
            m_function(function) {

        }

        T next() override {
            return std::invoke(m_function, m_a->next(), m_b->next());
        }

    private:
        RandomGenerator<T> *m_a;
        RandomGenerator<T> *m_b;
        Func m_function;
    };

    namespace detail {
        template <typename T>
        T mean(const T& a, const T& b) {
            return (a + b) / 2;
        }
    }

    template <typename T>
    class MeanRandomGenerator : public CombinedRandomGenerator<T, decltype(detail::mean<T>)> {
    public:
        MeanRandomGenerator(RandomGenerator<T> *a, RandomGenerator<T> *b) :
            CombinedRandomGenerator<T, T(const T&, const T&)>(a, b, detail::mean) {

        }
    };

    // template <typename T>
    // class MeanRandomGenerator : public CombinedRandomGenerator<T> {
    // public:
    //     MeanRandomGenerator(RandomGenerator<T> *a, RandomGenerator<T> *b) :
    //         CombinedRandomGenerator<T>(a, b, [](const T &t_1, const T &t_2) { return (t_1 + t_2) / 2; }) {
    //     }
    // };
}