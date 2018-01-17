#include "files/GameDescription.h"

#include <cctype>
#include <tmxlite/detail/Log.hpp>

namespace engine::files {
    void check_path_end(std::string &path) {
        if (path[path.size()-1] != '/' && path[path.size()-1] != '\\')
            path.push_back('/');
    }

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
                    l.output = tmx::Logger::Output::None;
                    tmx::Logger::log("game.json: log description: the value of 'output' is not valid. It must be \"none\", \"console\", \"file\" or \"all\". 'none' will be used as default", tmx::Logger::Type::Warning);
                }
            }
            else {
                tmx::Logger::log("game.json: log description: 'output' has an invalid type. It must be a string or an array. 'none' will be used as default.", tmx::Logger::Type::Warning);
                l.output = tmx::Logger::Output::None;
            }
        }
        else {
            tmx::Logger::log("game.json: log description does not the contain the 'output' field. 'None' will be used as default", tmx::Logger::Type::Warning);
            l.output = tmx::Logger::Output::None;
        }
    }

    void from_json(const nlohmann::json &j, GameDescription::WindowDescription &w) {
        auto fullscreen = j.find("fullscreen");
        if (fullscreen != j.end() && fullscreen->is_boolean())
            w.fullscreen = *fullscreen;
        else {
            w.fullscreen = false;
            tmx::Logger::log("game.json: window description does not contain the 'fullscreen' field or its type is not valid (should be a boolean). 'false' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto titlebar = j.find("showTitlebar");
        if (titlebar != j.end() && titlebar->is_boolean())
            w.titlebar = *titlebar;
        else {
            w.titlebar = true;
            tmx::Logger::log("game.json: window description does not contain the 'showTitlebar' field or its type is not valid (should be a boolean). 'false' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto resize = j.find("showResize");
        if (resize != j.end() && resize->is_boolean())
            w.resize = *resize;
        else {
            w.resize = true;
            tmx::Logger::log("game.json: window description does not contain the 'showResize' field or its type is not valid (should be a boolean). 'false' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto close = j.find("showClose");
        if (close != j.end() && close->is_boolean())
            w.close = *close;
        else {
            w.close = true;
            tmx::Logger::log("game.json: window description does not contain the 'showClose' field or its type is not valid (should be a boolean). 'false' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto engine = j.find("showEngine");
        if (engine != j.end() && engine->is_boolean())
            w.engine = *engine;
        else {
            w.engine = true;
            tmx::Logger::log("game.json: window description does not contain the 'showEngine' field or its type is not valid (should be a boolean). 'false' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto version = j.find("showVersion");
        if (version != j.end() && version->is_boolean())
            w.version = *version;
        else {
            w.version = true;
            tmx::Logger::log("game.json: window description does not contain the 'showVersion' field or its type is not valid (should be a boolean). 'false' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto title = j.find("title");
        if (title != j.end() && title->is_string())
            w.title = *title;
        else {
            w.title = "";
            tmx::Logger::log("game.json: window description does not contain the 'title' field or its type is not valid (should be a string). An empty string will be used as default.", tmx::Logger::Type::Warning);
        }

        auto size = j.find("size");
        if (size != j.end() && size->is_array()) {
            if (size->size() == 2 && (*size)[0].is_number_integer() && (*size)[1].is_number_integer()) {
                w.width = (*size)[0];
                w.height = (*size)[1];
            }
            else {
                w.width = 800;
                w.height = 600;
                tmx::Logger::log("game.json: window description: the 'size' field must be an array of 2 integers. [800, 600] will be used as default.", tmx::Logger::Type::Warning);
            }
        }
        else {
            w.width = 800;
            w.height = 600;
            tmx::Logger::log("game.json: window description does not contain the 'size' field or its type is not valid (it should be an array of 2 integers). [800, 600] will be used as default.", tmx::Logger::Type::Warning);
        }
    }

    void from_json(const nlohmann::json &j, GameDescription::MediaDescription &m) {
        auto baseSprites = j.find("baseSprites");
        if (baseSprites != j.end() && baseSprites->is_string()) {
            m.baseSprites = *baseSprites;
            check_path_end(m.baseSprites);
        }
        else {
            m.baseSprites = "media/sprites/";
            tmx::Logger::log("game.json: media description does not contain the 'baseSprites' field or its type is not valid (it should be a string). 'media/sprites/' will be used as default.", tmx::Logger::Type::Warning);
        }

        auto maps = j.find("maps");
        if (maps != j.end()) {
            if (maps->is_string()) {
                m.mapFolder = *maps;
            }
            else {
                m.mapFolder = "media/maps/";
                tmx::Logger::log("game.json: media description: the 'maps' field's type is not valid (it should be a string or an object). 'media/maps' will be used as default.", tmx::Logger::Type::Warning);
            }
        }
        else {
            m.mapFolder = "media/maps/";
            tmx::Logger::log("game.json: media description does not contain the 'maps' field. 'media/maps' will be used as default.", tmx::Logger::Type::Warning);
        }
    }

    void from_json(const nlohmann::json &j, GameDescription::StatesDescription &s) {
        auto first = j.find("first");
        if(first != j.end()) {
            std::string firstS = *first;
            std::transform(firstS.begin(), firstS.end(), firstS.begin(), ::tolower);
            s.firstState = firstS;
        }
    }

    void from_json(const nlohmann::json &j, GameDescription &g) {
        // We load the log description only in Debug build
        #ifdef _DEBUG_
        auto log = j.at("log");
        g.log = log;
        #endif
        auto window = j.at("window");
        g.window = window;
        auto media = j.at("media");
        g.media = media;
        auto states = j.at("states");
        g.states = states;
        auto version = j.at("version");
        g.version = version;
        auto name = j.at("name");
        g.name = name;
    }
}