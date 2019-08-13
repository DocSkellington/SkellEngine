#pragma once

#include <SFML/Graphics.hpp>

#include "SkellEngine/systems/System.h"

namespace engine::systems {
    /**
     * \brief Defines the base of every drawing system.
     * 
     * Every drawing system must start with "Graphical" to be correctly used
     */
    class GraphicalSystem : public System {
    public:
        /**
         * \brief A specific System::Ptr for graphical systems
         */
        typedef std::shared_ptr<GraphicalSystem> Ptr;
    
    public:
        /**
         * \brief The constructor
         * \param manager The system manager
         */
        explicit GraphicalSystem(SystemManager& manager);
        GraphicalSystem(const GraphicalSystem&) = delete;
        virtual ~GraphicalSystem();

        /**
         * \brief Renders the given layer in this system in the given window.
         * 
         * If the layer is not used in the system, nothing happens
         * \param target The target in which to render
         * \param layer The number of the layer to render
         * \param view The view of the game
         */
        virtual void draw(sf::RenderTarget &target, unsigned int layer, sf::View view) = 0;
    };
}