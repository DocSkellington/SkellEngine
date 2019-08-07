/**
 * \file to_from_sfml_events.h
 * Defines some functions string->sf::Event and sf::Event->string
 */
#pragma once

#include <map>

#include <SFML/Window/Event.hpp>

/**
 * \brief This namespace defines some functions used internally by the InputHandler
 */
namespace engine::input::detail {
    /**
     * \brief Converts a string to a SFML event type
     * \param type The string
     * \return The SFML event type
     */
    sf::Event::EventType stringToEventType(std::string type);

    std::string eventTypeToString(sf::Event::EventType type, bool isHold = false);
}