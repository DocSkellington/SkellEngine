#pragma once

#include "SkellEngine/systems/GraphicalSystem.h"
#include "SkellEngine/systems/SystemManager.h"

namespace engine::systems {
    /**
     * \brief The system that renders an orthogonal level
     */ 
    class GraphicalOrthogonalSystem : public GraphicalSystem {
    public:
        /**
         * \brief The constructor
         * \param manager The system manager
         */
        GraphicalOrthogonalSystem(SystemManager& manager);
        GraphicalOrthogonalSystem(const GraphicalOrthogonalSystem&) = delete;
        virtual ~GraphicalOrthogonalSystem();

        virtual bool update(sf::Int64 deltatime, sf::View &view) override;
        virtual void draw(sf::RenderTarget &target, unsigned int layer, sf::View view) override;

    protected:
        virtual bool checkComponents(entities::Entity::Ptr entity) const override;

        REGISTER_SYSTEM(GraphicalOrthogonalSystem, "graphicalorthogonal")
    };
}