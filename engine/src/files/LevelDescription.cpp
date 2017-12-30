#include "files/LevelDescription.h"

#include "errors/BadLevelDescription.h"

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
            std::cerr << "Error: LevelDescriptor " << l.name << " does not have a 'map' field or its type is not valid (it must be string)!";
            throw errors::BadLevelDescription("Error: LevelDescriptor " + l.name + " does not have a 'map' field or its type is not valid (it must be string)!");
        }

        if (entities != j.end() && entities->is_object()) {
            for (auto &entity : *entities) {
                l.entities.push_back(entity);
            }
        }
        else {
            std::cerr << "Error: LevelDescriptor does not have an 'entities' object!\n";
            throw errors::BadLevelDescription("Error: LevelDescriptor " + l.name + " does not have an 'entities' object!\n");
        }
    }
}