#include "SkellEngine/input/detail/write_json.h"

#include "SkellEngine/input/detail/to_from_sfml_events.h"
#include "SkellEngine/utilities/json_fusion.h"

namespace engine::input::detail {
    nlohmann::json writeInputDescriptionObject(const sf::Event &event, bool isHold, nlohmann::json payload, const std::string &state) {
        nlohmann::json json;

        json["type"] = eventTypeToString(event.type, isHold);
        json["state"] = state;

        // TODO: modifiers

        return utilities::json_fusion(json, payload);
    }
}