#pragma once

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

namespace engine::files {
    /**
     * \brief Describes a State of the game
     * 
     * It should be loaded trough JSON.
     */
    struct StateDescription {
        /**
         * \brief List the systems that must be loaded alongside the state
         */
        std::vector<std::string> systems;
    };
    
    /**
     * \brief Converts a JSON object to a state description
     */
    void from_json(const nlohmann::json &j, StateDescription &s);
}