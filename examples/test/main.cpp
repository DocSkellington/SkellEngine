#include "Engine.h"

#include "utilities/json_lua.h"

using namespace engine;

int main() {
    nlohmann::json json;
    json["test"] = "Hello";
    json["int"] = 55;
    sol::table lua = utilities::json_to_lua(json);

    Engine engine;
    engine.run();
    return 0;
}