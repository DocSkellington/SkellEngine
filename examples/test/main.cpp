#include "SkellEngine/Engine.h"

#include "SkellEngine/utilities/json_lua.h"

using namespace engine;

int main() {
    Engine engine("@CMAKE_CURRENT_SOURCE_DIR@/media");
    engine.run();
    return 0;
}