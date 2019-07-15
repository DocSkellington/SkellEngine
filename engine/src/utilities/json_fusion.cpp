#include "SkellEngine/utilities/json_fusion.h"

namespace engine::utilities {
    nlohmann::json json_fusion(const nlohmann::json &a, const nlohmann::json &b) {
        nlohmann::json result(a);
        for (auto itr = b.begin() ; itr != b.end() ; ++itr) {
            result[itr.key()] = itr.value();
        }
        return result;
    }
}