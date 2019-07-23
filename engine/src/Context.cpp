#include "SkellEngine/Context.h"

namespace engine {
    Context::Context() {}

    void Context::closeEngine() {
        tmx::Logger::log("Closing the window");
        window->close();
    }
}