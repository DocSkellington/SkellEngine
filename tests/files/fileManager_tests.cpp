#include <catch.hpp>

#include "SkellEngine/Context.h"
#include "SkellEngine/files/FileManager.h"

using namespace engine;

SCENARIO("File Manager", "[files][fileManager]") {
    Context context;
    context.fileManager = std::make_shared<files::FileManager>(context, "@CMAKE_CURRENT_SOURCE_DIR@/media");

    REQUIRE(context.fileManager->getGameDescription().media.fontsFolder == "@CMAKE_CURRENT_SOURCE_DIR@/media/path/to/fonts");
}