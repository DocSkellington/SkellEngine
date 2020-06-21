#pragma once

#include <random>
#include <list>
#include <typeinfo>

#include "SkellEngine/random/RandomGenerator.hpp"

namespace engine::random {
    /**
     * \brief A uniform random generator over a closed interval [a, b].
     * 
     * This RNG works for both integral (integer, long, char, and so on) and floating point types.
     * It also allows unscoped enumerations (i.e., non-class enumerations).
     * 
     * The rigged generation makes sure that the wanted value is produced often enough.
     * More precisely, let c be the wanted rigged value and p the period between two rigged values.
     * We make sure that at least one c appeared after p generated numbers.
     * For instance, if c = 1 and p = 2, then a possible sequence of generated numbers is [0, 5, 1, 9, 2, 1, 6, 1, 3 ...].
     * Notice that at least one 1 appears after every two generated numbers.
     * Of course, it may happen that multiple c are generated (due to randomness).
     * 
     * \warning If T is not an integral, nor a floating point, then the class does not compile
     * \see engine::random::RandomChoice to randomly select a value in a container using an uniform distribution.
     */
    template <typename T>
        requires std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_enum_v<T>
    class UniformRandomGenerator : public RandomGenerator<T> {
    public:
        /**
         * \brief A uniform generator that is not rigged 
         * \param a The lowest bound of the closed interval
         * \param b The upper bound of the closed interval
         */
        UniformRandomGenerator(T a, T b) :
            RandomGenerator<T>(false),
            m_distribution(a, getUpper(b)) {
        }

        /**
         * \brief A rigged uniform generator.
         * 
         * The period p is b - a.
         * 
         * \param a The lowest bound of the closed interval
         * \param b The upper bound of the closed interval
         * \param riggedValue The wanted number to occur often
         */
        UniformRandomGenerator(T a, T b, T riggedValue) :
            RandomGenerator<T>(true),
            m_distribution(a, getUpper(b)),
            m_riggedValue(riggedValue),
            m_period(b - a) {
        }

        /**
         * \brief A rigged uniform generator.
         * \param a The lowest bound of the closed interval
         * \param b The upper bound of the closed interval
         * \param riggedValue The wanted number to occur often
         * \param riggedPeriod The period of the riggedness
         */
        UniformRandomGenerator(T a, T b, T riggedValue, std::size_t riggedPeriod) :
            RandomGenerator<T>(true),
            m_distribution(a, getUpper(b)),
            m_riggedValue(riggedValue),
            m_period(riggedPeriod) {

        }

        virtual ~UniformRandomGenerator() = default;

        T next() override {
            if (this->isRigged()) {
                if (m_lastSamples.size() != m_period) {
                    T value = m_distribution(this->getGenerator());
                    m_lastSamples.push_back(value);
                    return value;
                }
                else {
                    // We rig the next number if we did not generate a m_riggedValue in the last samples
                    bool riggedSample = true;
                    for (const T &sample : m_lastSamples) {
                        if (sample == m_riggedValue) {
                            riggedSample = false;
                            break;
                        }
                    }

                    T value;
                    if (riggedSample) {
                        value = m_riggedValue;
                    }
                    else {
                        value = m_distribution(this->getGenerator());
                    }

                    m_lastSamples.pop_front();
                    m_lastSamples.push_back(value);

                    return value;
                }
            }
            else {
                return m_distribution(this->getGenerator());
            }
        }

    private:
        // We deduce the exact distribution type from T
        // That is, if T is an 'integer', then we use a uniform integer distribution
        // If T is neither an integer or a floating point, then this provokes a compilation error
        using distribution_type = std::conditional_t<
            std::is_floating_point_v<T>,
            std::uniform_real_distribution<T>,
            std::uniform_int_distribution<T>
        >;
    
    private:
        template<typename Q = T>
        typename std::enable_if_t<std::is_integral_v<Q> || std::is_enum_v<Q>, Q> getUpper(Q b) const {
            return b;
        }
        
        template<typename Q = T>
        typename std::enable_if_t<std::is_floating_point_v<Q>, Q> getUpper(Q b) const {
            return std::nextafter(b, std::numeric_limits<Q>::max());
        }

    private:
        distribution_type m_distribution;
        std::list<T> m_lastSamples;
        std::size_t m_period;
        T m_riggedValue;
    };
}