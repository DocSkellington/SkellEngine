#pragma once

#include <random>

/**
 * \brief Contains random number generators (RNG).
 * 
 * Some RNGs can be rigged in such a way that the perceived generation indeed follows the distribution.
 * For instance, a Bernoulli distribution with probability 1/3 will always produce at least one true value out of three samples.
 */
namespace engine::random {
    template <typename T>
    class RandomGenerator {
    public:
        RandomGenerator(const RandomGenerator &gen) = delete;

        virtual T next() = 0;

        T operator()() {
            return next();
        }

    protected:
        RandomGenerator(bool rigged) :
            m_rigged(rigged) {
            std::random_device rd;
            m_rng = std::mt19937_64(rd());
        }

        bool isRigged() {
            return m_rigged;
        }

        std::mt19937_64& getGenerator() {
            return m_rng;
        }

    private:
        bool m_rigged;
        std::mt19937_64 m_rng;
    };
}