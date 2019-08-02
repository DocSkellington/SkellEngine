#pragma once

#include "SkellEngine/systems/System.h"

#include <SFML/Graphics.hpp>

namespace engine::systems {
    /**
     * \brief Defines the base of every drawing system.
     * 
     * Every drawing system must start with "Graphical" to be correctly used
     */
    class GraphicalSystem : public System {
    public:
        typedef std::shared_ptr<GraphicalSystem> Ptr;
    
    public:
        explicit GraphicalSystem(SystemManager& manager);
        GraphicalSystem(const GraphicalSystem&) = delete;
        virtual ~GraphicalSystem();

        /**
         * \brief Renders the given layer in this system in the given window.
         * 
         * If the layer is not used in the system, nothing happens
         * \param window The window in which to render
         * \param layer The number of the layer to render
         * \param view The view of the game
         */
        virtual void draw(sf::RenderWindow *window, unsigned int layer, sf::View view) = 0;
    };
}