#include "files/GameDescription.h"

#include <cctype>
#include "log/Logger.h"

namespace engine::files {
    void check_path_end(std::string &path) {
        if (path[path.size()-1] != '/' && path[path.size()-1] != '\\')
            path.push_back('/');
    }

    void from_json(const nlohmann::json &j, GameDescription::LogDescription &l) {
        std::cout << "LOG DESCRIPTION\n";
        auto output = j.find("output"), level = j.find("level");

        if (output != j.end()) {
            if (output->is_string()) {
                std::string o = *output;
                if (o == "none")
                    l.output = log::LogOutput::OutputNone;
                else if (o == "console")
                    l.output = log::LogOutput::Console;
                else if (o == "file")
                    l.output = log::LogOutput::File;
                else if (o == "all")
                    l.output = log::LogOutput::OutputAll;
                else {
                    l.output = log::LogOutput::OutputAll;
                    log::log("game.json: log description: the value of 'output' is not valid. It must be \"none\", \"console\", \"file\" or \"all\". 'all' will be used as default", log::LogLevel::Warning);
                }
            }
            else if (output->is_array()) {
                l.output = log::LogOutput::OutputNone;
                for (auto &o : *output) {
                    if (o.is_string()) {
                        if (o == "none")
                            l.output = log::LogOutput::OutputNone;
                        else if (o == "console")
                            l.output = (log::LogOutput) (l.output | log::LogOutput::Console);
                        else if (o == "file")
                            l.output = (log::LogOutput) (l.output | log::LogOutput::File);
                        else if (o == "all")
                            l.output = log::LogOutput::OutputAll;
                        else {
                            l.output = log::LogOutput::OutputAll;
                            log::log("game.json: log description: the value of 'output' is not valid. It must be \"none\", \"console\", \"file\" or \"all\". 'all' will be used as default", log::LogLevel::Warning);
                            break;
                        }
                    }
                }
            }
            else {
                log::log("game.json: log description: 'output' has an invalid type. It must be a string or an array. 'all' will be used as default.", log::LogLevel::Warning);
                l.output = log::LogOutput::OutputAll;
            }
        }
        else {
            log::log("game.json: log description does not the contain the 'output' field. 'all' will be used as default", log::LogLevel::Warning);
            l.output = log::LogOutput::OutputAll;
        }

        if (level != j.end()) {
            if (level->is_string()) {
                std::string lev = *level;
                if (lev == "none")
                    l.level = log::LogLevel::LevelNone;
                else if (lev == "info")
                    l.level = log::LogLevel::Info;
                else if (lev == "warning")
                    l.level = log::LogLevel::Warning;
                else if (lev == "error")
                    l.level = log::LogLevel::Error;
                else if (lev == "all")
                    l.level = log::LogLevel::LevelAll;
                else {
                    l.level = log::LogLevel::LevelAll;
                    log::log("game.json: log description: the value of 'level' is not valid. It must be \"none\", \"info\", \"warning\", \"error\" or \"all\". 'all' will be used as default", log::LogLevel::Warning);
                }
            }
            else if (level->is_array()) {
                l.level = log::LogLevel::LevelNone;
                for (auto &lev : *level) {
                    if (lev.is_string()) {
                        if (lev == "none")
                            l.level = log::LogLevel::LevelNone;
                        else if (lev == "info")
                            l.level = (log::LogLevel) (l.level | log::LogLevel::Info);
                        else if (lev == "warning")
                            l.level = (log::LogLevel) (l.level | log::LogLevel::Warning);
                        else if (lev == "error")
                            l.level = (log::LogLevel) (l.level | log::LogLevel::Error);
                        else if (lev == "all")
                            l.level = log::LogLevel::LevelAll;
                        else {
                            l.level = log::LogLevel::LevelAll;
                            log::log("game.json: log description: the value of 'level' is not valid. It must be \"none\", \"info\", \"warning\", \"error\" or \"all\". 'all' will be used as default", log::LogLevel::Warning);
                            break;
                        }
                    }
                }
            }
            else {
                log::log("game.json: log description: 'level' has an invalid type. It must be a string or an array. 'all' will be used as default.", log::LogLevel::Warning);
                l.level = log::LogLevel::LevelAll;
            }
        }
        else {
            log::log("game.json: log description does not the contain the 'level' field. 'all' will be used as default", log::LogLevel::Warning);
            l.level = log::LogLevel::LevelAll;
        }
    }

    void from_json(const nlohmann::json &j, GameDescription::WindowDescription &w) {
        auto fullscreen = j.find("fullscreen");
        if (fullscreen != j.end() && fullscreen->is_boolean())
            w.fullscreen = *fullscreen;
        else {
            w.fullscreen = false;
            log::log("game.json: window description does not contain the 'fullscreen' field or its type is not valid (should be a boolean). 'false' will be used as default.", log::LogLevel::Warning);
        }

        auto titlebar = j.find("showTitlebar");
        if (titlebar != j.end() && titlebar->is_boolean())
            w.titlebar = *titlebar;
        else {
            w.titlebar = true;
            log::log("game.json: window description does not contain the 'showTitlebar' field or its type is not valid (should be a boolean). 'false' will be used as default.", log::LogLevel::Warning);
        }

        auto resize = j.find("showResize");
        if (resize != j.end() && resize->is_boolean())
            w.resize = *resize;
        else {
            w.resize = true;
            log::log("game.json: window description does not contain the 'showResize' field or its type is not valid (should be a boolean). 'false' will be used as default.", log::LogLevel::Warning);
        }

        auto close = j.find("showClose");
        if (close != j.end() && close->is_boolean())
            w.close = *close;
        else {
            w.close = true;
            log::log("game.json: window description does not contain the 'showClose' field or its type is not valid (should be a boolean). 'false' will be used as default.", log::LogLevel::Warning);
        }

        auto engine = j.find("showEngine");
        if (engine != j.end() && engine->is_boolean())
            w.engine = *engine;
        else {
            w.engine = true;
            log::log("game.json: window description does not contain the 'showEngine' field or its type is not valid (should be a boolean). 'false' will be used as default.", log::LogLevel::Warning);
        }

        auto version = j.find("showVersion");
        if (version != j.end() && version->is_boolean())
            w.version = *version;
        else {
            w.version = true;
            log::log("game.json: window description does not contain the 'showVersion' field or its type is not valid (should be a boolean). 'false' will be used as default.", log::LogLevel::Warning);
        }

        auto title = j.find("title");
        if (title != j.end() && title->is_string())
            w.title = *title;
        else {
            w.title = "";
            log::log("game.json: window description does not contain the 'title' field or its type is not valid (should be a string). An empty string will be used as default.", log::LogLevel::Warning);
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
                log::log("game.json: window description: the 'size' field must be an array of 2 integers. [800, 600] will be used as default.", log::LogLevel::Warning);
            }
        }
        else {
            w.width = 800;
            w.height = 600;
            log::log("game.json: window description does not contain the 'size' field or its type is not valid (it should be an array of 2 integers). [800, 600] will be used as default.", log::LogLevel::Warning);
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
            log::log("game.json: media description does not contain the 'baseSprites' field or its type is not valid (it should be a string). 'media/sprites/' will be used as default.", log::LogLevel::Warning);
        }

        auto maps = j.find("maps");
        if (maps != j.end()) {
            if (maps->is_string()) {
                m.maps[0] = m.maps[1] = m.maps[2] = *maps;
            }
            else if (maps->is_object()) {
                auto tilesets = maps->find("tilesets"), images = maps->find("images"), mapsPath = maps->find("maps");

                if (mapsPath != maps->end() && mapsPath->is_string()) {
                    m.maps[0] = *mapsPath;
                    check_path_end(m.maps[0]);
                }
                else {
                    m.maps[0] = "media/maps/";
                    log::log("game.json: media description: 'maps' does not contain the 'maps' field or its type is not valid (it should be a string). 'media/maps' will be used as default.", log::LogLevel::Warning);
                }

                if (tilesets != maps->end() && tilesets->is_string()) {
                    m.maps[1] = *tilesets;
                    check_path_end(m.maps[1]);
                }
                else {
                    m.maps[1] = m.maps[0];
                }

                if (images != maps->end() && images->is_string()) {
                    m.maps[2] = *images;
                    check_path_end(m.maps[2]);
                }
                else {
                    m.maps[2] = m.maps[0];
                }
            }
            else {
                m.maps[0] = m.maps[1] = m.maps[2] = "media/maps/";
                log::log("game.json: media description: the 'maps' field's type is not valid (it should be a string or an object). 'media/maps' will be used as default.", log::LogLevel::Warning);
            }
        }
        else {
            m.maps[0] = m.maps[1] = m.maps[2] = "media/maps/";
            log::log("game.json: media description does not contain the 'maps' field. 'media/maps' will be used as default.", log::LogLevel::Warning);
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
        auto log = j.at("log");
        g.log = log;
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