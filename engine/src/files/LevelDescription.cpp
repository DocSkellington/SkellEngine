#include "files/LevelDescription.h"

#include "errors/BadLevelDescription.h"
#include <tmxlite/detail/Log.hpp>

namespace engine::files {
    void LevelDescription::clear() {
        name = "";
        map = "";
        entities.clear();
    }

    void from_json(const nlohmann::json &j, LevelDescription &l) {
        auto map = j.find("map"), entities = j.find("entities");

        if (map != j.end() && map->is_string()) {
            l.map = *map;
        }
        else {
            throw errors::BadLevelDescription("LevelDescriptor does not have a 'map' field or its type is not valid (it must be string)!");
        }

        if (entities != j.end() && entities->is_object()) {
            for (auto &entity : *entities) {
                l.entities.push_back(entity);
            }
        }
        else {
            throw errors::BadLevelDescription("LevelDescriptor does not have an 'entities' object!\n");
        }
    }
}