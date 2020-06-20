#include <vector>
#include <map>

#include <catch.hpp>

#include "SkellEngine/random/RandomChoice.hpp"

#define NUMBER_ITERATIONS 1000

using namespace engine;
using namespace random;

enum SimpleEnumeration {
    A,
    B,
    C,
    D
};

enum class ClassEnumeration {
    A,
    B,
    C,
    D
};

SCENARIO("Random choice over a vector", "[random][choice]") {
    std::vector<int> vec = {5, 3, 1, 10};

    auto gen = RandomChoice(vec);
    
    for (int i = 0 ; i < NUMBER_ITERATIONS ; i++) {
        auto n = gen();
        REQUIRE(std::count(vec.begin(), vec.end(), n) == 1);
    }
}

SCENARIO("Random choice over an array", "[random][choice]") {
    std::array<char, 4> container = { 'A', 'B', 'C', 'D' };

    auto gen = RandomChoice(container);
    
    for (int i = 0 ; i < NUMBER_ITERATIONS ; i++) {
        auto n = gen();
        REQUIRE(std::count(container.begin(), container.end(), n) == 1);
    }
}

SCENARIO("Random choice over a map", "[random][choice]") {
    std::map<std::string, double> container = { {"pi", 3.1415}, {"e", 2.7182} };

    auto gen = RandomChoice(container);
    
    for (int i = 0 ; i < NUMBER_ITERATIONS ; i++) {
        auto n = gen();
        REQUIRE(std::count(container.begin(), container.end(), n) == 1);
    }
}