#include "SkellEngine/files/GameDescription.hpp"

#include <cctype>
#include <filesystem>

namespace sol {
    template<>
    struct is_container<std::filesystem::path> : std::false_type {};
}

namespace engine::files {
    void GameDescription::luaFunctions(sol::state &lua) {
        using path = std::filesystem::path;
        lua.new_usertype<path>("Path",
            sol::constructors<path(const std::string&), path(const path&)>(),
            sol::meta_function::to_string, &path::native,
            sol::meta_function::division, sol::overload(
                [](const path &left, const path &right) {
                    return left / right;
                },
                [](const path& left, const std::string& right) {
                    return left / right;
                }
            ),
            "append", static_cast<path& (path::*)(const std::string&)>(&path::append)
        );

        lua.new_usertype<GameDescription::MediaDescription>("MediaDescription",
            sol::no_constructor,
            "baseSprites",      sol::readonly_property(&MediaDescription::baseSprites),
            "mapFolder",        sol::readonly_property(&MediaDescription::mapFolder),
            "systemsFolder",    sol::readonly_property(&MediaDescription::systemsFolder),
            "fontsFolder",      sol::readonly_property(&MediaDescription::fontsFolder),
            "entitiesFolder",   sol::readonly_property(&MediaDescription::entitiesFolder),
            "levelsFolder",     sol::readonly_property(&MediaDescription::levelsFolder),
            "statesFolder",     sol::readonly_property(&MediaDescription::statesFolder),
            "inputDescription", sol::readonly_property(&MediaDescription::inputDescription),
            "baseMediaPath",    sol::readonly_property(&MediaDescription::baseMediaPath)
        );

        lua.new_usertype<GameDescription>("GameDescription",
            sol::no_constructor,
            "media", &GameDescription::media,
            "version", &GameDescription::version,
            "name", &GameDescription::name
        );
    }

    void from_json(const nlohmann::json &j, GameDescription::LogDescription &l) {
        if (auto file = j.find("file") ; file != j.end() && file->is_string()) {
            if (file->is_string()) {
                l.logPath = file->get<std::string>();
            }
            else {
                Logger::forceLog("game.json: log description: 'file' must be a string. 'log.txt' will be used as default.");
            }
        }

        if (auto level = j.find("level") ; level != j.end()) {
            if (level->is_string()) {
                const std::string &lev = level->get<std::string>();
                if (lev == "info") {
                    l.level = LogType::Info;
                }
                else if (lev == "warning") {
                    l.level = LogType::Warning;
                }
                else if (lev == "error") {
                    l.level = LogType::Error;
                }
                else {
                    Logger::forceLog("game.json: log description: the value of 'level' is invalid. It must be \"info\", \"warning\" or \"error\". \"info\" is used by default.");
                }
            }
            else {
                Logger::forceLog("game.json: log description: 'level' has an invalid type. It must be a string. 'info' is used by default.");
            }
        }

        if (auto output = j.find("output") ; output != j.end()) {
            if (output->is_string()) {
                const std::string& o = output->get<std::string>();
                if (o == "console") {
                    l.output = LogOutput::Console;
                }
                else if (o == "file") {
                    l.output = LogOutput::File;
                }
                else if (o == "all") {
                    l.output = LogOutput::All;
                }
                else if (o == "none") {
                    l.output = LogOutput::None;
                }
                else {
                    Logger::forceLog("game.json: log description: the value of 'output' is not valid. It must be \"none\", \"console\", \"file\" or \"all\". 'none' will be used as default");
                }
            }
            else {
                Logger::forceLog("game.json: log description: 'output' has an invalid type. It must be a string or an array. 'none' will be used as default.");
            }
        }
    }

    void from_json(const nlohmann::json &j, GameDescription::WindowDescription &w) {
        if (auto fullscreen = j.find("fullscreen") ; fullscreen != j.end()) {
            if (fullscreen->is_boolean()) {
                w.fullscreen = fullscreen->get<bool>();
            }
            else {
                Logger::forceLog("game.json: window description: 'fullscreen' must be a boolean. 'false' will be used as default.");
            }
        }

        if (auto titlebar = j.find("showTitlebar") ; titlebar != j.end()) {
            if (titlebar->is_boolean()) {
                w.titlebar = titlebar->get<bool>();
            }
            else {
                Logger::forceLog("game.json: window description: 'showTitlebar' must be a boolean. 'true' will be used as default.");
            }
        }

        
        if (auto resize = j.find("showResize") ; resize != j.end()) {
            if (resize->is_boolean()) {
                w.resize = resize->get<bool>();
            }
            else {
                Logger::forceLog("game.json: window description: 'showResize' must be a boolean. 'true' will be used as default.");
            }
        }

        if (auto close = j.find("showClose") ; close != j.end()) {
            if (close->is_boolean()) {
                w.close = close->get<bool>();
            }
            else {
                Logger::forceLog("game.json: window description: 'showClose' must be a boolean. 'true' will be used as default.");
            }
        }

        if (auto engine = j.find("showEngine") ; engine != j.end()) {
            if (engine->is_boolean()) {
                w.engine = engine->get<bool>();
            }
            else {
                Logger::forceLog("game.json: window description: 'showEngine' must be a boolean. 'true' will be used as default.");
            }
        }

        if (auto version = j.find("showVersion") ; version != j.end()) {
            if (version->is_boolean()) {
                w.version = version->get<bool>();
            }
            else {
                Logger::forceLog("game.json: window description: 'showVersion' must be a boolean. 'true' will be used as default.");
            }
        }

        if (auto title = j.find("title") ; title != j.end()) {
            if (title->is_string()) {
                w.title = title->get<std::string>();
            }
            else {
                Logger::forceLog("game.json: window description: 'title' must be a string. An empty string will be used as default.");
            }
        }

        if (auto size = j.find("size") ; size != j.end()) {
            if (size->is_array() && size->size() == 2 && (*size)[0].is_number_integer() && (*size)[1].is_number_integer()) {
                w.width = (*size)[0].get<int>();
                w.height = (*size)[1].get<int>();
            }
            else {
                Logger::forceLog("game.json: window description: the 'size' field must be an array of 2 integers. [800, 600] will be used as default.");
            }
        }

        if (auto antialiasing = j.find("antialiasingLevel") ; antialiasing != j.end()) {
            if (antialiasing->is_number_integer()) {
                w.antialiasingLevel = antialiasing->get<int>();
            }
            else {
                Logger::forceLog("game.json: window description: 'antialiasingLevel' must be an integer. 0 will be used as default.");
            }
        }

        if (auto FPS = j.find("FPS") ; FPS != j.end()) {
            if (FPS->is_number_integer()) {
                w.FPS = FPS->get<int>();
            }
            else {
                Logger::forceLog("game.json: window description: 'FPS' must be an integer. 60 will be used as default.");
            }
        }

        if (auto verticalSync = j.find("verticalSynchronisation") ; verticalSync != j.end()) {
            if (verticalSync->is_boolean()) {
                w.verticalSynchronisation = verticalSync->get<bool>();
            }
            else {
                Logger::forceLog("game.json: window description: 'verticalSynchronisation' must be a boolean. 'false' will be used as default.");
            }
        }
    }

    void from_json(const nlohmann::json &j, GameDescription::MediaDescription &m) {
        if (auto baseSprites = j.find("baseSprites") ; baseSprites != j.end()) {
            if (baseSprites->is_string()) {
                m.baseSprites = baseSprites->get<std::string>();
            }
            else {
                Logger::forceLog("game.json: media description: 'baseSprites' must be a string. 'sprites/' will be used as default.");
            }
        }

        auto maps = j.find("maps");
        if (maps != j.end()) {
            if (maps->is_string()) {
                m.mapFolder = maps->get<std::string>();
            }
            else {
                Logger::forceLog("game.json: media description: 'maps' must be a string . 'maps/' will be used as default.");
            }
        }

        if (auto systems = j.find("systems") ; systems != j.end()) {
            if (systems->is_string()) {
                m.systemsFolder = systems->get<std::string>();
            }
            else {
                Logger::forceLog("game.json: media description: 'systems' must be a string. 'systems/' will be used as default");
            }
        }

        if (auto fonts = j.find("fonts") ; fonts != j.end()) {
            if (fonts->is_string()) {
                m.fontsFolder = fonts->get<std::string>();
            }
            else {
                Logger::forceLog("game.json: media description: 'fonts' must be a string. 'fonts/' will be used as default");
            }
        }

        if (auto entities = j.find("entities") ; entities != j.end()) {
            if (entities->is_string()) {
                m.entitiesFolder = entities->get<std::string>();
            }
            else {
                Logger::forceLog("game.json: media description: 'entities' must be a string. 'entities/' will be used as default");
            }
        }

        if (auto levels = j.find("levels") ; levels != j.end()) {
            if (levels->is_string()) {
                m.levelsFolder = levels->get<std::string>();
            }
            else {
                Logger::forceLog("game.json: media description: 'levels' must be a string. 'levels/' will be used as default");
            }
        }

        
        if (auto states = j.find("states") ; states != j.end()) {
            if (states->is_string()) {
                m.statesFolder = states->get<std::string>();
            }
            else {
                Logger::forceLog("game.json: media description: 'states' must be a string. 'states/' will be used as default");
            }
        }

        if (auto input = j.find("input") ; input != j.end()) {
            if (input->is_string()) {
                m.inputDescription = input->get<std::string>();
            }
            else {
                Logger::forceLog("game.json: media description: 'input' must be a string. 'keys.json' will be used as default");
            }
        }
    }

    void from_json(const nlohmann::json &j, GameDescription::StatesDescription &s) {
        if(auto first = j.find("first") ; first != j.end()) {
            if (first->is_string()) {
                std::string firstS = first->get<std::string>();
                std::transform(firstS.begin(), firstS.end(), firstS.begin(), ::tolower);
                s.firstState = firstS;
            }
            else {
                Logger::forceLog("game.json: states description: 'first' must be a string. 'intro' will be used as default value.");
            }
        }
    }

    void from_json(const nlohmann::json &j, GameDescription &g) {
        // We load the log description only in Debug build
        if (auto log = j.find("log") ; log != j.end()) {
            if (log->is_object()) {
                g.log = log->get<GameDescription::LogDescription>();
            }
            else {
                Logger::forceLog("game.json: the log description must be an object. Default values will be used");
            }
        }

        if (auto window = j.find("window") ; window != j.end()) {
            if (window->is_object()) {
                g.window = window->get<GameDescription::WindowDescription>();
            }
            else {
                Logger::forceLog("game.json: the window description must be an object. Default values will be used");
            }
        }

        if (auto media = j.find("media") ; media != j.end()) {
            if (media->is_object()) {
                g.media = media->get<GameDescription::MediaDescription>();
            }
            else {
                Logger::forceLog("game.json: the media description must be an object. Default values will be used");
            }
        }

        if (auto states = j.find("states") ; states != j.end()) {
            if (states->is_object()) {
                g.states = states->get<GameDescription::StatesDescription>();
            }
            else {
                Logger::forceLog("game.json: the states description must be an object. Default values will be used");
            }
        }

        if (auto version = j.find("version") ; version != j.end()) {
            if (version->is_string()) {
                g.version = version->get<std::string>();
            }
            else {
                g.version = "";
                Logger::forceLog("game.json: the 'version' field must be a string. An empty string will be used as default");
            }
        }

        if (auto name = j.find("name") ; name != j.end()) {
            if (name->is_string()) {
                g.name = name->get<std::string>();
            }
            else {
                Logger::forceLog("game.json: the 'name' field must be a string. An empty string will be used as default");
            }
        }
    }
}