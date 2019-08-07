#pragma once

#include <nlohmann/json.hpp>
#include <SFML/Window/Event.hpp>

namespace engine::input::detail {
    nlohmann::json writeInputDescriptionObject(const sf::Event &event, bool isHold, nlohmann::json payload, const std::string &state);
}