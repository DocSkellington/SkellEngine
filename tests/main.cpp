#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include <SkellEngine/tmxlite/Log.hpp>

int main(int argc, char* argv[]) {
    tmx::Logger::setOutput(tmx::Logger::Output::Console);
    int result = Catch::Session().run(argc, argv);
    return result;
}