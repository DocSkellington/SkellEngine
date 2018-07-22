#include <catch.hpp>

#include "utilities/json_lua.h"

using namespace engine::utilities;

TEST_CASE("JSON to Lua table", "[utilities]") {
    sol::state luaState;

    SECTION("A JSON can be just an array") {
        nlohmann::json json = {1,3};

        auto table = json_to_lua(json, luaState);

        REQUIRE(table[0] == 1);
        REQUIRE(table[1] == 3);
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
        REQUIRE(array.get<long>(0) == 1);
        REQUIRE(array.get<long>(1) == 3);
        REQUIRE(array.get<long>(2) == 5);
        REQUIRE(array.get<double>(3) == Approx(8.9));
        REQUIRE(array.get<std::string>(4) == "string");
    }

    SECTION("Nested arrays") {
        nlohmann::json json = {
            {"array", {3, {true, "hello"}}}
        };

        auto table = json_to_lua(json, luaState);

        REQUIRE(table["array"][0] == 3);
        REQUIRE(table["array"][1][0].get<bool>());
        REQUIRE(table["array"][1][1].get<std::string>() == "hello");
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

    SECTION("Nested unnamed tables") {
        luaState.script(R"(
            table = {
                5, {3.14, true, "string"}, false
            }
        )");

        nlohmann::json json = lua_to_json(luaState["table"]);

        REQUIRE(json[1] == 5);
        REQUIRE(json[2][1] == Approx(3.14));
        REQUIRE(json[2][2]);
        REQUIRE(json[2][3] == "string");
        REQUIRE_FALSE(json[3]);
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

    SECTION("Nested named tables") {
        luaState.script(R"(
            table = {
                pi = 3.14,
                level2 = {
                    string = "string",
                    bool = true,
                    level3 = {
                        notBool = "true",
                        notString = false
                    }
                },
                pi2 = 6.28
            }
        )");

        nlohmann::json json = lua_to_json(luaState["table"]);

        REQUIRE(json["pi"] == Approx(3.14));
        REQUIRE(json["level2"]["string"] == "string");
        REQUIRE(json["level2"]["bool"]);
        REQUIRE(json["level2"]["level3"]["notBool"] == "true");
        REQUIRE_FALSE(json["level2"]["level3"]["notString"]);
        REQUIRE(json["pi2"] == Approx(6.28));
    }
}