#include <catch.hpp>
#include <iostream>

#include "SkellEngine/math/Vector2.hpp"

using namespace engine;

SCENARIO("Vector2's functions are correct", "[vector2][math]") {
    Vector2d vec = {10, 7};

    WHEN("We add a single number") {
        auto vec2 = vec + 4;
        REQUIRE(vec2 == Vector2d(14, 11));
    }

    WHEN("We add two vectors together") {
        Vector2d vec2 = {7, 10};
        auto vec3 = vec + vec2;
        REQUIRE(vec3 == Vector2d(17, 17));
    }

    WHEN("We multiply a vector by a number") {
        auto vec2 = vec * 3;
        REQUIRE(vec2 == Vector2d(30, 21));
    }

    WHEN("We multiply two vectors together") {
        Vector2d vec2 = {5, 8};
        REQUIRE(vec * vec2 == 106);
    }
}