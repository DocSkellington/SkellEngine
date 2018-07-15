#include <catch.hpp>

#include "utilities/json_lua.h"

using namespace engine::utilities;

TEST_CASE("JSON to Lua table", "[utilities]") {
    nlohmann::json json = {
        {"pi", 3.14}
    };
    sol::state luaState;

    std::cout << json << '\n';

    SECTION("Test") {
        sol::table lua = luaState.create_table();
        json_to_lua(json, lua);

        REQUIRE(lua.get<float>("pi") == 3.14);
    }
}