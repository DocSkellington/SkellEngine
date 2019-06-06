#include "SkellEngine/files/StateDescription.h"

#include <algorithm>
#include <cctype>

#include <SkellEngine/tmxlite/Log.hpp>

namespace engine::files {
    void from_json(const nlohmann::json &j, StateDescription &s) {
        auto systems = j.find("systems");
        if (systems != j.end() && systems->is_array()) {
            for (std::size_t i = 0 ; i < systems->size() ; i++) {
                if ((*systems)[i].is_string()) {
                    std::string system = (*systems)[i];
                    std::transform(system.begin(), system.end(), system.begin(), ::tolower);
                    s.systems.push_back(system);
                }
                else {
                    tmx::Logger::log("every system defined in a state description must be a string.", tmx::Logger::Type::Warning);
                }
            }
        }
    }
}