#include <iostream>
#include <catch.hpp>

#include "SkellEngine/Context.h"
#include "SkellEngine/files/FileManager.h"

using namespace engine;
using namespace files;

TEST_CASE("Game description loading", "[json_description]") {
    nlohmann::json json;
    SECTION("A game.json that overrides every option") {
        std::ifstream file("media/game.json");
        file >> json;
        GameDescription game = json.get<GameDescription>();

        SECTION("Log Description is verified") {
            GameDescription::LogDescription log = game.log;
            SECTION ("'output' is correct") {
                REQUIRE(log.output == tmx::Logger::Output::All);
            }
        }

        SECTION("Window Description is verified") {
            GameDescription::WindowDescription win = game.window;
            SECTION("'fullscreen' is correct") {
                REQUIRE(win.fullscreen);
            }
            SECTION("'showTitlebar' is correct") {
                REQUIRE_FALSE(win.titlebar);
            }
            SECTION("'showResize' is correct") {
                REQUIRE_FALSE(win.resize);
            }
            SECTION("'showClose' is correct") {
                REQUIRE_FALSE(win.close);
            }
            SECTION("'showVersion' is correct") {
                REQUIRE_FALSE(win.version);
            }
            SECTION("'showEngine' is correct") {
                REQUIRE_FALSE(win.engine);
            }
            SECTION("'size' is correct") {
                REQUIRE(win.width == 5241);
                REQUIRE(win.height == 4952);
            }
            SECTION("'title' is correct") {
                REQUIRE(win.title == "Testy");
            }
            SECTION("'antialiasinglevel' is correct") {
                REQUIRE(win.antialiasingLevel == 8);
            }
        }

        SECTION("Media description is verified") {
            GameDescription::MediaDescription media = game.media;
            REQUIRE(media.baseSprites == "path/to/sprite/");
            REQUIRE(media.mapFolder == "path/to/maps/");
            REQUIRE(media.systemsFolder == "path/to/systems/");
        }

        SECTION("States description is verified") {
            GameDescription::StatesDescription states = game.states;
            SECTION("'first' is correct") {
                REQUIRE(states.firstState == "mainmenu");
            }
        }

        SECTION("Base fields are verified") {
            SECTION("'version' is correct") {
                REQUIRE(game.version == "Test Version");
            }
            SECTION("'name' is correct") {
                REQUIRE(game.name == "Catch2");
            }
        }
    }

    SECTION("The file is empty") {
        std::ifstream file("media/emptyGame.json");
        file >> json;
        GameDescription game = json.get<GameDescription>();

        SECTION("Log description is initialised by default") {
            GameDescription::LogDescription log = game.log;
            REQUIRE(log.output == tmx::Logger::Output::None);
        }
        SECTION("Window description is initialised by default") {
            GameDescription::WindowDescription win = game.window;
            REQUIRE(win.antialiasingLevel == 0);
            REQUIRE(win.close);
            REQUIRE(win.engine);
            REQUIRE_FALSE(win.fullscreen);
            REQUIRE(win.resize);
            REQUIRE(win.titlebar);
            REQUIRE(win.version);
            REQUIRE(win.title == "");
            REQUIRE(win.width == 800);
            REQUIRE(win.height == 600);
        }
        SECTION("Media description is initialised by default") {
            GameDescription::MediaDescription media = game.media;
            REQUIRE(media.baseSprites == "media/sprites/");
            REQUIRE(media.mapFolder == "media/maps/");
        }
        SECTION("States description is initialiased by default") {
            GameDescription::StatesDescription states = game.states;
            REQUIRE(states.firstState == "intro");
        }
        SECTION("Base fields are initialised by default") {
            REQUIRE(game.version == "");
            REQUIRE(game.name == "");
        }
    }
}