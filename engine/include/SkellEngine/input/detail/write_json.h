#pragma once

#include <nlohmann/json.hpp>
#include <SFML/Window/Event.hpp>

namespace engine::input::detail {
    /**
     * \brief Writes the given configuration in a JSON format
     * \param event The event
     * \param isHold Whether the key is hold
     * \param payload The payload
     * \param state The state in which the input binding is active
     * \param lalt Whether left alt must be hold
     * \param lshift Whether left shift must be hold
     * \param lcontrol Whether left control must be hold
     * \param ralt Whether right alt must be hold
     * \param rshift Whether right shift must be hold
     * \param rcontrol Whether right control must be hold
     * \return The JSON object
     */
    nlohmann::json writeInputDescriptionObject(const sf::Event &event, bool isHold, nlohmann::json payload, const std::string &state, bool lalt, bool lshift, bool lcontrol, bool ralt, bool rshift, bool rcontrol);

    /**
     * \brief Writes the configuration for a keyboard event
     * \param json The JSON in wich to write
     * \param event The event
     */
    void writeInputDescriptionObjectKeyboard(nlohmann::json &json, const sf::Event &event);
    /**
     * \brief Writes the configuration for a mouse button event
     * \param json The JSON in wich to write
     * \param event The event
     */
    void writeInputDescriptionObjectMouseButton(nlohmann::json &json, const sf::Event &event);
    /**
     * \brief Writes the configuration for a joystick button event
     * \param json The JSON in wich to write
     * \param event The event
     */
    void writeInputDescriptionObjectJoystickButton(nlohmann::json &json, const sf::Event &event);
    /**
     * \brief Writes the configuration for a joystick move event
     * \param json The JSON in wich to write
     * \param event The event
     */
    void writeInputDescriptionObjectJoystickMove(nlohmann::json &json, const sf::Event &event);
}