#include <catch.hpp>

#include "utilities/json_lua.h"

using namespace engine::utilities;

TEST_CASE("JSON to Lua table", "[utilities]") {
    sol::state luaState;

    SECTION("A JSON can be just an array") {
        nlohmann::json json = {1,3};

        auto table = json_to_lua(json, luaState);

        REQUIRE(table[1] == 1);
    }

    SECTION("Simple object") {
        nlohmann::json json = {
            {"pi", 3.14},
            {"string", "this is a string"},
            {"unsigned integer", 5},
            {"integer", -8},
            {"boolean", true},
            {"array", {1, 3, 5, 8.9, "string"}}
        };

        auto table = json_to_lua(json, luaState);

        REQUIRE(table.get<double>("pi") == Approx(3.14));
        REQUIRE(table.get<unsigned long>("unsigned integer") == 5);
        REQUIRE(table.get<long>("integer") == -8);
        REQUIRE(table.get<std::string>("string") == "this is a string");
        REQUIRE(table.get<bool>("boolean"));
        
        sol::table array = table["array"];
        REQUIRE(array.get<long>(1) == 1);
        REQUIRE(array.get<long>(2) == 3);
        REQUIRE(array.get<long>(3) == 5);
        REQUIRE(array.get<double>(4) == Approx(8.9));
        REQUIRE(array.get<std::string>(5) == "string");
    }

    SECTION("Nested arrays") {
        nlohmann::json json = {
            {"array", {3, {true, "hello"}}}
        };

        auto table = json_to_lua(json, luaState);

        REQUIRE(table["array"][1] == 3);
        REQUIRE(table["array"][2][1]);
        REQUIRE(table["array"][2][2] == "hello");
    }

    SECTION("Nested objects") {
        nlohmann::json json {
            {
                "level1", {
                    {"someInt", 5},
                    {
                        "level2", {
                            {"someStr", "someStr"},
                            {"someFloat", 1.23}
                        }
                    },
                    {"someBool", false}
                }
            }
        };

        sol::table table = json_to_lua(json, luaState);

        sol::table level1 = table["level1"];

        REQUIRE(level1["someInt"] == 5);
        REQUIRE_FALSE(level1["someBool"]);

        sol::table level2 = level1["level2"];
        REQUIRE(level2.get<std::string>("someStr") == "someStr");
        REQUIRE(level2.get<double>("someFloat") == Approx(1.23));
    }
}

TEST_CASE("Lua table to JSON", "[utilities]") {
    sol::state luaState;

    SECTION("Just a table") {
        luaState.script(R"(
            table = {
                5, "string", true
            }
        )");

        nlohmann::json json = lua_to_json(luaState["table"]);

        REQUIRE(json[1] == 5);
        REQUIRE(json[2] == "string");
        REQUIRE(json[3]);
    }

    SECTION("Simple objects") {
        luaState.script(R"(
            table = {
                19.56,
                bool = false,
                42,
                string = 'someString'
            }
        )");

        auto json = lua_to_json(luaState["table"]);

        SECTION ("Wrong index type") {
            REQUIRE_THROWS(json[1]);
        }

        SECTION ("Correct index type") {
            REQUIRE_FALSE(json["bool"]);
            REQUIRE(json["1"] == Approx(19.56));
            REQUIRE(json["2"] == 42);
            REQUIRE(json["string"] == "someString");
        }
    }
}