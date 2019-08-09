#include <catch.hpp>

#include "SkellEngine/Context.h"
#include "SkellEngine/input/InputHandler.h"

using namespace engine;
using namespace input;

SCENARIO("Input handler save and load configuration", "[input]") {
    Context context;

    InputHandler handler(context);

    WHEN("We load a configuration with one fully-defined Closed input binding with payload") {
        auto configuration =
        "{"\
            "\"testEvent\": {"\
                "\"type\": \"closed\","\
                "\"state\": \"all\","\
                "\"lalt\": false,"\
                "\"lshift\": false,"\
                "\"lcontrol\": true,"\
                "\"ralt\": true,"\
                "\"rshift\": false,"\
                "\"rcontrol\": false,"\
                "\"payload1:\": 10,"\
                "\"payload2:\": \"TEST\""\
            "}"\
        "}"_json;
        REQUIRE_NOTHROW(handler.loadConfiguration(configuration));

        THEN("We get a similar configuration when saving") {
            auto saved = handler.saveConfiguration();

            REQUIRE(configuration == saved);
        }
    }

    WHEN ("We load a configuration with one KeyPressed input binding") {
        auto configuration =
        "{"\
            "\"testEvent\": {"\
                "\"type\": \"keypressed\","\
                "\"key\": \"Up\""\
            "}"\
        "}"_json;
        REQUIRE_NOTHROW(handler.loadConfiguration(configuration));

        THEN("We get a similar configuration when saving") {
            auto saved = handler.saveConfiguration();

            REQUIRE(configuration["testEvent"]["type"] == saved["testEvent"]["type"]);
            REQUIRE(configuration["testEvent"]["key"] == saved["testEvent"]["key"]);
        }
    }

    WHEN ("We load a configuration with one MouseButtonReleased input binding") {
        auto configuration =
        "{"\
            "\"testEvent\": {"\
                "\"type\": \"mousebuttonreleased\","\
                "\"mouse button\": \"Middle\""\
            "}"\
        "}"_json;
        REQUIRE_NOTHROW(handler.loadConfiguration(configuration));

        THEN("We get a similar configuration when saving") {
            auto saved = handler.saveConfiguration();

            REQUIRE(configuration["testEvent"]["type"] == saved["testEvent"]["type"]);
            REQUIRE(configuration["testEvent"]["mouse button"] == saved["testEvent"]["mouse button"]);
        }
    }

    WHEN ("We load a configuration with one JoystickButtonHold input binding") {
        auto configuration =
        "{"\
            "\"testEvent\": {"\
                "\"type\": \"joystickbuttonhold\","\
                "\"joystick id\": 0,"\
                "\"joystick button\": 0"\
            "}"\
        "}"_json;
        REQUIRE_NOTHROW(handler.loadConfiguration(configuration));

        THEN("We get a similar configuration when saving") {
            auto saved = handler.saveConfiguration();

            REQUIRE(configuration["testEvent"]["type"] == saved["testEvent"]["type"]);
            REQUIRE(configuration["testEvent"]["id"] == saved["testEvent"]["id"]);
            REQUIRE(configuration["testEvent"]["joystick button"] == saved["testEvent"]["joystick button"]);
        }
    }

    WHEN ("We load a configuration with one JoystickMoved input binding") {
        auto configuration =
        "{"\
            "\"testEvent\": {"\
                "\"type\": \"joystickmoved\","\
                "\"joystick id\": 0,"\
                "\"axis\": \"X\""\
            "}"\
        "}"_json;
        REQUIRE_NOTHROW(handler.loadConfiguration(configuration));

        THEN("We get a similar configuration when saving") {
            auto saved = handler.saveConfiguration();

            REQUIRE(configuration["testEvent"]["type"] == saved["testEvent"]["type"]);
            REQUIRE(configuration["testEvent"]["id"] == saved["testEvent"]["id"]);
            REQUIRE(configuration["testEvent"]["axis"] == saved["testEvent"]["axis"]);
        }
    }

    WHEN ("We load a configuration using an array (an OR operation) with one input") {
        auto configuration =
        "{"\
            "\"testEvent\": ["\
                 "{"\
                    "\"type\": \"joystickmoved\","\
                    "\"joystick id\": 1,"\
                    "\"axis\": \"Y\","\
                    "\"state\": \"all\","\
                    "\"lalt\": false,"\
                    "\"lshift\": false,"\
                    "\"lcontrol\": true,"\
                    "\"ralt\": true,"\
                    "\"rshift\": false,"\
                    "\"rcontrol\": false"\
                "}"\
            "]"
        "}"_json;

        REQUIRE_NOTHROW(handler.loadConfiguration(configuration));

        THEN ("The saved configuration does not use an array") {
            auto saved = handler.saveConfiguration();

            REQUIRE(saved["testEvent"].is_object());
            REQUIRE(saved["testEvent"] == configuration["testEvent"][0]);
        }
    }

    WHEN ("We load a configuration with an array (an OR operation) with multiple events") {
        auto configuration =
        "{"\
            "\"testEvent\": ["\
                 "{"\
                    "\"type\": \"joystickmoved\","\
                    "\"joystick id\": 1,"\
                    "\"axis\": \"Y\","\
                    "\"state\": \"all\","\
                    "\"lalt\": false,"\
                    "\"lshift\": false,"\
                    "\"lcontrol\": true,"\
                    "\"ralt\": true,"\
                    "\"rshift\": false,"\
                    "\"rcontrol\": false"\
                "},"\
                "{"\
                    "\"type\": \"closed\","\
                    "\"state\": \"all\","\
                    "\"lalt\": false,"\
                    "\"lshift\": true,"\
                    "\"lcontrol\": false,"\
                    "\"ralt\": false,"\
                    "\"rshift\": true,"\
                    "\"rcontrol\": true"\
                "}"\
            "]"
        "}"_json;

        REQUIRE_NOTHROW(handler.loadConfiguration(configuration));

        THEN ("The saved configuration is similar") {
            auto saved = handler.saveConfiguration();

            REQUIRE(saved["testEvent"].is_array());
            REQUIRE(saved == configuration);
        }
    }
}