#include <catch.hpp>

#include "utilities/json_lua.h"

using namespace engine::utilities;

TEST_CASE("JSON to Lua table", "[utilities]") {
    sol::state luaState;

    /*
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
            {"array", {1, 3, 5, 8.9}}
        };

        auto table = json_to_lua(json, luaState);

        REQUIRE(table.get<float>("pi") == Approx(3.14));
        REQUIRE(table.get<unsigned long>("unsigned integer") == 5);
        REQUIRE(table.get<long>("integer") == -8);
        REQUIRE(table.get<std::string>("string") == "this is a string");
        REQUIRE(table.get<bool>("boolean"));
        
        sol::table array = table["array"];
        REQUIRE(array.get<long>(1) == 1);
        REQUIRE(array.get<long>(2) == 3);
        REQUIRE(array.get<long>(3) == 5);
        REQUIRE(array.get<float>(4) == Approx(8.9));
    }
    */

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
        //REQUIRE(level2["someStr"] == "someStr");
        REQUIRE(level2["someFloat"] == Approx(1.23));
    }
}