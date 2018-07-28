#include "files/GameDescription.h"

#include <cctype>
#include <tmxlite/Log.hpp>

namespace engine::files {
    void check_path_end(std::string &path) {
        if (path[path.size()-1] != '/' && path[path.size()-1] != '\\')
            path.push_back('/');
    }

    GameDescription::LogDescription defaultLog {tmx::Logger::Output::None};

    void from_json(const nlohmann::json &j, GameDescription::LogDescription &l) {
        auto output = j.find("output");

        if (output != j.end()) {
            if (output->is_string()) {
                std::string o = *output;
                if (o == "console")
                    l.output = tmx::Logger::Output::Console;
                else if (o == "file")
                    l.output = tmx::Logger::Output::File;
                else if (o == "all")
                    l.output = tmx::Logger::Output::All;
                else if (o == "none")
                    l.output = tmx::Logger::Output::None;
                else {
                    l.output = defaultLog.output;
                    tmx::Logger::log("game.json: log description: the value of 'output' is not valid. It must be \"none\", \"console\", \"file\" or \"all\". 'none' will be used as default", tmx::Logger::Type::Warning);
                }
            }
            else {
                tmx::Logger::log("game.json: log description: 'output' has an invalid type. It must be a string or an array. 'none' will be used as default.", tmx::Logger::Type::Warning);
                l.output = defaultLog.output;
            }
        }
        else {
            tmx::Logger::log("game.json: log description does not the contain the 'output' field. 'None' will be used as default", tmx::Logger::Type::Warning);
            l.output = defaultLog.output;
        }
    }

    GameDescription::WindowDescription defaultWindow {false, true, true, true, true, true, "", 800, 600, 0};

    void from_json(const nlohmann::json &j, GameDescription::WindowDescription &w) {
        auto fullscreen = j.find("fullscreen");
        if (fullscreen != j.end() && fullscreen->is_boolean())
            w.fullscreen = *fullscreen;
        else {
            w.fullscreen = defaultWindow.fullscreen;
            tmx::Logger::log("game.json: window description does not contain the 'fullscreen' field or its type is not valid (should be a boolean). 'false' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto titlebar = j.find("showTitlebar");
        if (titlebar != j.end() && titlebar->is_boolean())
            w.titlebar = *titlebar;
        else {
            w.titlebar = defaultWindow.titlebar;
            tmx::Logger::log("game.json: window description does not contain the 'showTitlebar' field or its type is not valid (should be a boolean). 'true' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto resize = j.find("showResize");
        if (resize != j.end() && resize->is_boolean())
            w.resize = *resize;
        else {
            w.resize = defaultWindow.resize;
            tmx::Logger::log("game.json: window description does not contain the 'showResize' field or its type is not valid (should be a boolean). 'true' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto close = j.find("showClose");
        if (close != j.end() && close->is_boolean())
            w.close = *close;
        else {
            w.close = defaultWindow.close;
            tmx::Logger::log("game.json: window description does not contain the 'showClose' field or its type is not valid (should be a boolean). 'true' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto engine = j.find("showEngine");
        if (engine != j.end() && engine->is_boolean())
            w.engine = *engine;
        else {
            w.engine = defaultWindow.engine;
            tmx::Logger::log("game.json: window description does not contain the 'showEngine' field or its type is not valid (should be a boolean). 'true' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto version = j.find("showVersion");
        if (version != j.end() && version->is_boolean())
            w.version = *version;
        else {
            w.version = defaultWindow.version;
            tmx::Logger::log("game.json: window description does not contain the 'showVersion' field or its type is not valid (should be a boolean). 'true' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto title = j.find("title");
        if (title != j.end() && title->is_string())
            w.title = *title;
        else {
            w.title = defaultWindow.title;
            tmx::Logger::log("game.json: window description does not contain the 'title' field or its type is not valid (should be a string). An empty string will be used as default.", tmx::Logger::Type::Warning);
        }

        auto size = j.find("size");
        if (size != j.end() && size->is_array()) {
            if (size->size() == 2 && (*size)[0].is_number_integer() && (*size)[1].is_number_integer()) {
                w.width = (*size)[0];
                w.height = (*size)[1];
            }
            else {
                w.width = defaultWindow.width;
                w.height = defaultWindow.height;
                tmx::Logger::log("game.json: window description: the 'size' field must be an array of 2 integers. [800, 600] will be used as default.", tmx::Logger::Type::Warning);
            }
        }
        else {
            w.width = defaultWindow.width;
            w.height = defaultWindow.height;
            tmx::Logger::log("game.json: window description does not contain the 'size' field or its type is not valid (it should be an array of 2 integers). [800, 600] will be used as default.", tmx::Logger::Type::Warning);
        }

        auto antialiasing = j.find("antialiasingLevel");
        if (antialiasing != j.end() && antialiasing->is_number()) {
            w.antialiasingLevel = *antialiasing;
        }
        else {
            w.antialiasingLevel = defaultWindow.antialiasingLevel;
            tmx::Logger::log("game.json: window description does not contain the 'antialiasingLevel' field or its type is not valid (it should be an integer). 0 will be used as default.");
        }
    }

    GameDescription::MediaDescription defaultMedia {"media/sprites/", "media/maps/", "media/systems/"};

    void from_json(const nlohmann::json &j, GameDescription::MediaDescription &m) {
        auto baseSprites = j.find("baseSprites");
        if (baseSprites != j.end() && baseSprites->is_string()) {
            m.baseSprites = *baseSprites;
            check_path_end(m.baseSprites);
        }
        else {
            m.baseSprites = defaultMedia.baseSprites;
            tmx::Logger::log("game.json: media description does not contain the 'baseSprites' field or its type is not valid (it should be a string). 'media/sprites/' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto maps = j.find("maps");
        if (maps != j.end()) {
            if (maps->is_string()) {
                m.mapFolder = *maps;
                check_path_end(m.mapFolder);
            }
            else {
                m.mapFolder = defaultMedia.mapFolder;
                tmx::Logger::log("game.json: media description: the 'maps' field's type is not valid (it should be a string or an object). 'media/maps' will be used as default.", tmx::Logger::Type::Warning);
            }
        }
        else {
            m.mapFolder = defaultMedia.mapFolder;
            tmx::Logger::log("game.json: media description does not contain the 'maps' field. 'media/maps' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto systems = j.find("systems");
        if (systems != j.end() && systems->is_string()) {
            m.systemsFolder = *systems;
            check_path_end(m.systemsFolder);
        }
        else {
            m.systemsFolder = defaultMedia.systemsFolder;
            tmx::Logger::log("game.json: media description does not contain the 'systems' field or its type is not valid (it should be a string). 'media/systems/' will be used as default", tmx::Logger::Type::Warning);
        }
    }

    GameDescription::StatesDescription defaultState {"intro"};

    void from_json(const nlohmann::json &j, GameDescription::StatesDescription &s) {
        auto first = j.find("first");
        if(first != j.end()) {
            std::string firstS = *first;
            std::transform(firstS.begin(), firstS.end(), firstS.begin(), ::tolower);
            s.firstState = firstS;
        }
        else {
            s.firstState = defaultState.firstState;
            tmx::Logger::log("game.json: states description does not contain the 'first' field or its type is not valid (it should be a string). 'intro' will be used as default value.", tmx::Logger::Type::Warning);
        }
    }

    void from_json(const nlohmann::json &j, GameDescription &g) {
        // We load the log description only in Debug build
        auto log = j.find("log");
        if (log != j.end() && log->is_object()) {
            g.log = *log;
        }
        else {
            g.log = defaultLog;
            tmx::Logger::log("game.json: the log description is not present. Default values will be used", tmx::Logger::Type::Warning);
        }

        auto window = j.find("window");
        if (window != j.end() && window->is_object()) {
            g.window = *window;
        }
        else {
            g.window = defaultWindow;
            tmx::Logger::log("game.json: the window description is not present. Default values will be used", tmx::Logger::Type::Warning);
        }

        auto media = j.find("media");
        if (media != j.end() && media->is_object()) {
            g.media = *media;
        }
        else {
            g.media = defaultMedia;
            tmx::Logger::log("game.json: the media description is not present. Default values will be used", tmx::Logger::Type::Warning);
        }

        auto states = j.find("states");
        if (states != j.end() && states->is_object()) {
            g.states = *states;
        }
        else {
            g.states = defaultState;
            tmx::Logger::log("game.json: the states description is not present. Default values will be used", tmx::Logger::Type::Warning);
        }

        auto version = j.find("version");
        if (version != j.end() && version->is_string()) {
            g.version = *version;
        }
        else {
            g.version = "";
            tmx::Logger::log("game.json: the 'version' field is not present or is not a string. An empty string will be used as default");
        }

        auto name = j.find("name");
        if (name != j.end() && name->is_string()) {
            g.name = *name;
        }
        else {
            g.name = "";
            tmx::Logger::log("game.json: the 'name' field is not present or is not a string. An empty string will be used as default");
        }
    }
}