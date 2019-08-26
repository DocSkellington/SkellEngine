#include "SkellEngine/levels/LevelDescription.h"

#include "SkellEngine/errors/BadLevelDescription.h"

namespace engine::levels {
    LevelDescription::EntityDescription::EntityDescription(const std::string &name, const nlohmann::json &description) :
        name(name),
        description(description) {

    }

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
            for (const auto &[key, value] : entities->items()) {
                l.entities.emplace_back(key, value);
            }
        }
        else {
            throw errors::BadLevelDescription("LevelDescriptor does not have an 'entities' object!\n");
        }
    }
}