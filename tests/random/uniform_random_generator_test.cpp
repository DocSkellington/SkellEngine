#include <iostream>

#include <catch.hpp>

#include "SkellEngine/random/UniformRandomGenerator.hpp"
#include "SkellEngine/random/CombinedRandomGenerator.hpp"

using namespace engine;
using namespace random;

#define NUMBER_ITERATIONS 1000

SCENARIO("Uniform random generator on integers", "[random][uniform]") {
    GIVEN("A non-rigged generator") {
        UniformRandomGenerator<long> uniform(-5, 10);

        for (int i = 0 ; i < NUMBER_ITERATIONS ; i++) {
            auto n = uniform.next();
            REQUIRE(-5 <= n);
            REQUIRE(n <= 10);
        }
    }

    GIVEN("A rigged generator") {
        UniformRandomGenerator<int> uniform(-9, 0, -5);

        std::list<int> lastSamples;
        for (int i = 0 ; i < NUMBER_ITERATIONS ; i++) {
            auto n = uniform.next();
            if (i > 0 && i % 10 == 0) {
                REQUIRE(std::count(lastSamples.begin(), lastSamples.end(), -5) > 0);
            }

            if (i > 10) {
                lastSamples.pop_front();
            }
            lastSamples.push_back(n);
        }
    }
}

SCENARIO("Uniform random generator on floating points", "[random][uniform]") {
    GIVEN("A non-rigged generator") {
        double lower = 2.71828;
        double upper = 3.1415;
        UniformRandomGenerator<double> uniform(lower, upper);

        for (int i = 0 ; i < NUMBER_ITERATIONS ; i++) {
            auto n = uniform.next();
            REQUIRE(lower <= n);
            REQUIRE(n <= upper);
        }
    }

    GIVEN("A rigged generator (with an explicit period)") {
        UniformRandomGenerator<double> uniform(0, 10, -5, 3);

        std::list<int> lastSamples;
        for (int i = 0 ; i < NUMBER_ITERATIONS ; i++) {
            auto n = uniform.next();
            std::cout << n << "\n";
            if (i > 10) {
                lastSamples.pop_front();
            }
            lastSamples.push_back(n);

            if (i > 0 && i % 3 == 0) {
                REQUIRE(std::count(lastSamples.begin(), lastSamples.end(), -5) > 0);
            }

        }
    }
}

SCENARIO("Mean of two uniform random generators", "[random][uniform][mean]") {
    UniformRandomGenerator<long> a(-5, 10);
    UniformRandomGenerator<long> b(0, 3);
    // CombinedRandomGenerator gen(&a, &b, [](const long &t_1, const long &t_2) { return (t_1 + t_2) / 2; });
    MeanRandomGenerator gen(&a, &b);

    for (int i = 0 ; i < NUMBER_ITERATIONS ; i++) {
        auto n = gen();
        std::cout << n << "\n";
        REQUIRE((-5 + 0) / 2 <= n);
        REQUIRE(n <= (10 + 3) / 2);
    }
}