#pragma once

#include <iostream>
#include <vector>
#include <array>

#include "SkellEngine/random/RandomGenerator.hpp"
#include "SkellEngine/random/UniformRandomGenerator.hpp"

namespace engine::random {
    /**
     * \brief A random generator over a given container.
     * 
     * Each element of the container has the same probability to appear.
     * 
     * Note that the container is copied. This means that you can modify or delete the given container, as the generator has its own version.
     * 
     * In intern, it relies on a UniformRandomGenerator. Thus, the way the generation is rigged follows immediately from that generator.
     */
    template <typename Container, typename T = typename Container::value_type>
    class RandomChoice : public RandomGenerator<T> {
    public:
        /**
         * \brief A choice generator that is not rigged 
         * \param container The container in which to pick the values
         */
        RandomChoice(const Container &container) :
            RandomGenerator<T>(false),
            m_container(container),
            m_indexGenerator(0, container.size() - 1) {
        }

        /**
         * \brief A choice generator that is not rigged 
         * \param container The container in which to pick the values
         * \param riggedIndex The index of the element that must appear more often
         */
        RandomChoice(const Container &container, std::size_t riggedIndex) :
            RandomGenerator<T>(true),
            m_container(container),
            m_indexGenerator(0, container.size() - 1, riggedIndex) {
        }

        /**
         * \brief A choice generator that is not rigged 
         * \param container The container in which to pick the values
         * \param riggedIndex The index of the element that must appear more often
         * \param riggedPeriod The number of choices to generate before forcing the element at riggedIndex to appear
         */
        RandomChoice(const Container &container, std::size_t riggedIndex, std::size_t riggedPeriod) :
            RandomGenerator<T>(true),
            m_container(container),
            m_indexGenerator(0, container.size() - 1, riggedIndex, riggedPeriod) {
        }

        virtual ~RandomChoice() = default;

        T next() override {
            // The rigged generation is directly handled by the underlying uniform generator
            auto itr = m_container.begin();
            for (auto nextIndex = m_indexGenerator() ; nextIndex != 0 ; nextIndex--, itr++);
            return *itr;
        }

    private:
        Container m_container;
        UniformRandomGenerator<std::size_t> m_indexGenerator;
    };

    /**
     * \brief A specialized RandomChoice generator for a vector.
     * 
     * The way the next value is generated is more efficient than in the general version.
     * 
     * \see engine::random::RandomChoice for the documentation of a RandomChoice generator.
     */
    template <typename T>
    class RandomChoice<std::vector<T>> : public RandomGenerator<T> {
    public:
        /**
         * \brief A choice generator that is not rigged 
         * \param container The container in which to pick the values
         */
        RandomChoice(const std::vector<T> &container) :
            RandomGenerator<T>(false),
            m_container(container),
            m_indexGenerator(0, container.size() - 1) {
        }

        /**
         * \brief A choice generator that is not rigged 
         * \param container The container in which to pick the values
         * \param riggedIndex The index of the element that must appear more often
         */
        RandomChoice(const std::vector<T> &container, std::size_t riggedIndex) :
            RandomGenerator<T>(true),
            m_container(container),
            m_indexGenerator(0, container.size() - 1, riggedIndex) {
        }

        /**
         * \brief A choice generator that is not rigged 
         * \param container The container in which to pick the values
         * \param riggedIndex The index of the element that must appear more often
         * \param riggedPeriod The number of choices to generate before forcing the element at riggedIndex to appear
         */
        RandomChoice(const std::vector<T> &container, std::size_t riggedIndex, std::size_t riggedPeriod) :
            RandomGenerator<T>(true),
            m_container(container),
            m_indexGenerator(0, container.size() - 1, riggedIndex, riggedPeriod) {
        }

        virtual ~RandomChoice() = default;

        T next() override {
            // The rigged generation is directly handled by the underlying uniform generator
            auto nextIndex = m_indexGenerator();
            return m_container[nextIndex];
        }

    private:
        std::vector<T> m_container;
        UniformRandomGenerator<std::size_t> m_indexGenerator;
    };

    /**
     * \brief A specialized RandomChoice generator for an array.
     * 
     * The way the next value is generated is more efficient than in the general version.
     * 
     * \see engine::random::RandomChoice for the documentation of a RandomChoice generator.
     */
    template <typename T, std::size_t N>
    class RandomChoice<std::array<T, N>> : public RandomGenerator<T> {
    public:
        /**
         * \brief A choice generator that is not rigged 
         * \param container The container in which to pick the values
         */
        RandomChoice(const std::array<T, N> &container) :
            RandomGenerator<T>(false),
            m_container(container),
            m_indexGenerator(0, container.size() - 1) {
        }

        /**
         * \brief A choice generator that is not rigged 
         * \param container The container in which to pick the values
         * \param riggedIndex The index of the element that must appear more often
         */
        RandomChoice(const std::array<T, N> &container, std::size_t riggedIndex) :
            RandomGenerator<T>(true),
            m_container(container),
            m_indexGenerator(0, container.size() - 1, riggedIndex) {
        }

        /**
         * \brief A choice generator that is not rigged 
         * \param container The container in which to pick the values
         * \param riggedIndex The index of the element that must appear more often
         * \param riggedPeriod The number of choices to generate before forcing the element at riggedIndex to appear
         */
        RandomChoice(const std::array<T, N> &container, std::size_t riggedIndex, std::size_t riggedPeriod) :
            RandomGenerator<T>(true),
            m_container(container),
            m_indexGenerator(0, container.size() - 1, riggedIndex, riggedPeriod) {
        }

        virtual ~RandomChoice() = default;

        T next() override {
            // The rigged generation is directly handled by the underlying uniform generator
            auto nextIndex = m_indexGenerator();
            return m_container[nextIndex];
        }

    private:
        std::array<T, N> m_container;
        UniformRandomGenerator<std::size_t> m_indexGenerator;
    };
}