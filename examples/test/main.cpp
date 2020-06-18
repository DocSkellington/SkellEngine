#include "SkellEngine/Engine.hpp"

#include "SkellEngine/utilities/json_lua.hpp"

using namespace engine;

int main() {
    Engine engine("@CMAKE_CURRENT_SOURCE_DIR@/media");
    engine.run();
    return 0;
}