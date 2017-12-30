#pragma once

#include <string>
#include <vector>

#include <json.hpp>

namespace engine::files {
    struct LevelDescription {
        std::string name;
        std::string map;
        std::vector<nlohmann::json> entities;

        void clear();
    };

    // Throws BadLevelDescription is a field is missing or of the wrong type
    void from_json(const nlohmann::json &j, LevelDescription &l);
}