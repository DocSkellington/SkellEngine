#include <catch.hpp>

#include "SkellEngine/Context.h"
#include "SkellEngine/files/FileManager.h"
#include "SkellEngine/errors/FileNotFound.h"

using namespace engine;

SCENARIO("The file manager is loaded when the media path is correct", "[files][fileManager]") {
    Context context("@CMAKE_CURRENT_SOURCE_DIR@/media", false);

    REQUIRE(context.fileManager->getGameDescription().media.fontsFolder == "@CMAKE_CURRENT_SOURCE_DIR@/media/path/to/fonts");
}

SCENARIO("The file manager is not loaded and throws an exception when the media path is incorrect", "[files][fileManager]") {
    REQUIRE_THROWS_AS(Context("@CMAKE_CURRENT_SOURCE_DIR@/false_media", false), errors::FileNotFound);
    // REQUIRE_THROWS(Context context("", false));
}