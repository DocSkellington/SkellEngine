#pragma once

#include <SFML/Window/Event.hpp>

namespace engine::input::detail {
    //@{
    /**
     * \brief The default keys
     */
    const sf::Keyboard::Key AnyKey = sf::Keyboard::Key::KeyCount;
    const sf::Mouse::Button AnyMouseButton = sf::Mouse::Button::ButtonCount;
    const int AnyJoystickButton = sf::Joystick::ButtonCount;
    const int AnyJoystickID = sf::Joystick::Count;
    //@}

    /**
     * \brief The default key/button is the "Any" key/button
     * \param event The event to modify
     */
    void setDefaultKeyButton(sf::Event &event);
}