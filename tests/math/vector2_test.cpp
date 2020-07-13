#include <catch.hpp>
#include <sstream>

#include "SkellEngine/math/Vector2.hpp"

using namespace engine;

struct Dummy {
    Dummy(float v) :
        value(v) {}
    float value;
};

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
        REQUIRE((vec * vec2) == 106);
    }

    WHEN("We compare two vectors of different types") {
        Vector2i vec2 = {3, 0};
        REQUIRE(vec > vec2);
        REQUIRE(vec2 < vec);
        REQUIRE(vec != vec2);
    }

    WHEN("We use an input stream") {
        Vector2i vec2;
        std::string values = "7 91";
        std::stringstream stream(values);
        stream >> vec2;
        REQUIRE(vec2 == Vector2i(7, 91));
    }

    Vector2<Dummy> v {Dummy{5.3}, Dummy{4.7}};
    REQUIRE(v.x.value == 5.3f);

    Vector2<Dummy> v2 {Dummy{7}, Dummy{2}};
    // Should not compile:
    // Vector2<Dummy> v3;
    // v + v2;
    // v * v2;
}