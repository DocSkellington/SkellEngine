#pragma once

#include <vector>
#include <string>

#include <json.hpp>

namespace engine::files {
    struct StateDescription {
        std::vector<std::string> systems;
    };
    
    void from_json(const nlohmann::json &j, StateDescription &s);
}