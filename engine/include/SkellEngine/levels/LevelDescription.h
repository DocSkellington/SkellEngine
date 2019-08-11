#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace engine::levels {
    /**
     * \brief Describes a level.
     * 
     * It should be loaded trough JSON (except for the level's name).
     */
    struct LevelDescription {
        /**
         * \brief The name of the level
         */
        std::string name;
        /**
         * \brief The path to the map relative to GameDescription::MediaDescription::mapFolder
         */
        std::string map;
        /**
         * \brief The list with every entity in the level
         * 
         * More entities can be added later
         */
        std::vector<nlohmann::json> entities;

        /**
         * \brief Clears the level description by resetting every field
         */
        void clear();
    };

    /**
     * \brief Converts a JSON object to a level description
     * \throws BadLevelDescription is a field is missing or of the wrong type
     */
    void from_json(const nlohmann::json &j, LevelDescription &l);
}