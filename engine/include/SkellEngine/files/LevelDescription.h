#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace engine::files {
    /**
     * \brief Describes a level.
     * 
     * It should be loaded trough JSON (except for the level's name).
     */
    struct LevelDescription {
        std::string name;
        std::string map;
        std::vector<nlohmann::json> entities;

        /**
         * \brief Clears the level description by resetting every field
         */
        void clear();
    };

    /** \throws BadLevelDescription is a field is missing or of the wrong type */
    void from_json(const nlohmann::json &j, LevelDescription &l);
}