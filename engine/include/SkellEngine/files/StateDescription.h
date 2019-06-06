#pragma once

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

namespace engine::files {
    /**
     * \brief Describes a State of the game
     * \ingroup Engine
     * 
     * It should be loaded trough JSON.
     */
    struct StateDescription {
        std::vector<std::string> systems;
    };
    
    void from_json(const nlohmann::json &j, StateDescription &s);
}