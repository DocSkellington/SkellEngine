#pragma once

#include "SkellEngine/systems/GraphicalSystem.h"

namespace engine::systems {
    /**
     * \brief The system that renders an orthogonal level
     */ 
    class GraphicalOrthogonalSystem : public GraphicalSystem {
    public:
        GraphicalOrthogonalSystem(SystemManager& manager);
        GraphicalOrthogonalSystem(const GraphicalOrthogonalSystem&) = delete;
        virtual ~GraphicalOrthogonalSystem();

        virtual bool update(sf::Int64 deltatime, sf::View &view) override;
        virtual void draw(sf::RenderWindow* window, unsigned int layer, sf::View view) override;

    protected:
        virtual bool checkComponents(entities::Entity::Ptr entity) const override;

        REGISTER_SYSTEM(GraphicalOrthogonalSystem, "graphicalorthogonal")
    };
}