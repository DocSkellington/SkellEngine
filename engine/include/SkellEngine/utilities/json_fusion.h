#pragma once

#include <nlohmann/json.hpp>

namespace engine::utilities {
    /**
     * \brief Fuses a and b. Be warn that b overrides data in a!
     * \param a The base json
     * \param b The overwriting json
     * \return The fusion of a and b (where common keys have b values)
    */
    nlohmann::json json_fusion(const nlohmann::json &a, const nlohmann::json &b);
}