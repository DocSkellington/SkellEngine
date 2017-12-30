#include "files/GameDescription.h"

#include <cctype>

namespace engine::files {
    void check_path_end(std::string &path) {
        if (path[path.size()-1] != '/' && path[path.size()-1] != '\\')
            path.push_back('/');
    }

    void from_json(const nlohmann::json &j, GameDescription::WindowDescription &w) {
        auto fullscreen = j.find("fullscreen");
        if (fullscreen != j.end() && fullscreen->is_boolean())
            w.fullscreen = *fullscreen;
        else {
            w.fullscreen = false;
            std::cerr << "Warning: game.json: window description does not contain the 'fullscreen' field or its type is not valid (should be a boolean). 'false' will be used as default.\n";
        }

        auto titlebar = j.find("showTitlebar");
        if (titlebar != j.end() && titlebar->is_boolean())
            w.titlebar = *titlebar;
        else {
            w.titlebar = true;
            std::cerr << "Warning: game.json: window description does not contain the 'showTitlebar' field or its type is not valid (should be a boolean). 'false' will be used as default.\n";
        }

        auto resize = j.find("showResize");
        if (resize != j.end() && resize->is_boolean())
            w.resize = *resize;
        else {
            w.resize = true;
            std::cerr << "Warning: game.json: window description does not contain the 'showResize' field or its type is not valid (should be a boolean). 'false' will be used as default.\n";
        }

        auto close = j.find("showClose");
        if (close != j.end() && close->is_boolean())
            w.close = *close;
        else {
            w.close = true;
            std::cerr << "Warning: game.json: window description does not contain the 'showClose' field or its type is not valid (should be a boolean). 'false' will be used as default.\n";
        }

        auto engine = j.find("showEngine");
        if (engine != j.end() && engine->is_boolean())
            w.engine = *engine;
        else {
            w.engine = true;
            std::cerr << "Warning: game.json: window description does not contain the 'showEngine' field or its type is not valid (should be a boolean). 'false' will be used as default.\n";
        }

        auto version = j.find("showVersion");
        if (version != j.end() && version->is_boolean())
            w.version = *version;
        else {
            w.version = true;
            std::cerr << "Warning: game.json: window description does not contain the 'showVersion' field or its type is not valid (should be a boolean). 'false' will be used as default.\n";
        }

        auto title = j.find("title");
        if (title != j.end() && title->is_string())
            w.title = *title;
        else {
            w.title = "";
            std::cerr << "Warning: game.json: window description does not contain the 'title' field or its type is not valid (should be a string). An empty string will be used as default.\n";
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
                std::cerr << "Warning: game.json: window description: the 'size' field must be an array of 2 integers. [800, 600] will be used as default.\n";
            }
        }
        else {
            w.width = 800;
            w.height = 600;
            std::cerr << "Warning: game.json: window description does not contain the 'size' field or its type is not valid (it should be an array of 2 integers). [800, 600] will be used as default.\n";
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
            std::cerr << "Warning: game.json: media description does not contain the 'baseSprites' field or its type is not valid (it should be a string). 'media/sprites/' will be used as default.\n";
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
                    std::cerr << "Warning: game.json: media description: 'maps' does not contain the 'maps' field or its type is not valid (it should be a string). 'media/maps' will be used as default.\n";
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
                std::cerr << "Warning: game.json: media description: the 'maps' field's type is not valid (it should be a string or an object). 'media/maps' will be used as default.\n";
            }
        }
        else {
            m.maps[0] = m.maps[1] = m.maps[2] = "media/maps/";
            std::cerr << "Warning: game.json: media description does not contain the 'maps' field. 'media/maps' will be used as default.\n";
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