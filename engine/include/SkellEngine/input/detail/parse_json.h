#pragma once

#include <SFML/Window/Event.hpp>
#include <nlohmann/json.hpp>

namespace engine::input::detail {
    /**
     * \brief Parses a JSON object and creates a sf::Event from that JSON object
     * 
     * It also sets different values needed by the input handler
     * \param inputDescription The input description. Used fields are removed from the object
     * \param event The event to create
     * \param hold Whether the event must be sent as long as the key is pressed
     * \param lalt Whether left alt must be pressed for the event to be processed
     * \param lshift Whether left shift must be pressed for the event to be processed
     * \param lcontrol Whether left control must be pressed for the event to be processed
     * \param ralt Whether right alt must be pressed for the event to be processed
     * \param rshift Whether right shift must be pressed for the event to be processed
     * \param rcontrol Whether right control must be pressed for the event to be processed
     * \param state The state in which the event is active
     * \param payload The payload of the event
     * \throws errors::InvalidJSON if the inputDescription is invalid
     */
    void parseInputDescriptionObject(nlohmann::json &inputDescription, sf::Event &event, bool &hold, bool &lalt, bool &lshift, bool &lcontrol, bool &ralt, bool &rshift, bool &rcontrol, std::string &state, nlohmann::json &payload);

    /**
     * \brief Parses a JSON object to extract the keyboard inputs
     * \param inputDescription The input description. Used fields are removed from the object
     * \param event The event to create
     * \throws errors::InvalidJSON if the inputDescription is invalid
     */
    inline void parseInputDescriptionObjectKeyboard(nlohmann::json &inputDescription, sf::Event &event);

    /**
     * \brief Parses a JSON object to extract the mouse button inputs
     * \param inputDescription The input description. Used fields are removed from the object
     * \param event The event to create
     * \throws errors::InvalidJSON if the inputDescription is invalid
     */
    inline void parseInputDescriptionObjectMouseButton(nlohmann::json &inputDescription, sf::Event &event);

    /**
     * \brief Parses a JSON object to extract the joystick button inputs
     * \param inputDescription The input description. Used fields are removed from the object
     * \param event The event to create
     * \throws errors::InvalidJSON if the inputDescription is invalid
     */
    inline void parseInputDescriptionObjectJoystickButton(nlohmann::json &inputDescription, sf::Event &event);

    /**
     * \brief Parses a JSON object to extract the joystick move inputs
     * \param inputDescription The input description. Used fields are removed from the object
     * \param event The event to create
     * \throws errors::InvalidJSON if the inputDescription is invalid
     */
    inline void parseInputDescriptionObjectJoystickMove(nlohmann::json &inputDescription, sf::Event &event);

    /**
     * \brief Parses a JSON object to extract the joystick move inputs
     * \param inputDescription The input description. Used fields are removed from the object
     * \param lalt Whether left alt must be pressed for the event to be processed
     * \param lshift Whether left shift must be pressed for the event to be processed
     * \param lcontrol Whether left control must be pressed for the event to be processed
     * \param ralt Whether right alt must be pressed for the event to be processed
     * \param rshift Whether right shift must be pressed for the event to be processed
     * \param rcontrol Whether right control must be pressed for the event to be processed
     * \throws errors::InvalidJSON if the inputDescription is invalid
     */
    inline void parseInputDescriptionObjectModifiers(nlohmann::json &inputDescription, bool &lalt, bool &lshift, bool &lcontrol, bool &ralt, bool &rshift, bool &rcontrol);
}