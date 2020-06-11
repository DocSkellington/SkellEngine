#include "SkellEngine/utilities/json_fusion.hpp"

namespace engine::utilities {
    nlohmann::json json_fusion(const nlohmann::json &a, const nlohmann::json &b) {
        nlohmann::json result(a);

        if (!b.is_object() || b.size() == 0) {
            return result;
        }

        for (auto itr = b.begin() ; itr != b.end() ; ++itr) {
            result[itr.key()] = itr.value();
        }
        return result;
    }
}