#include "SkellEngine/systems/GraphicalOrthogonalSystem.hpp"

#include <memory>
#include <iostream>

#include "SkellEngine/entities/components/GraphicalSpriteSheetComponent.hpp"
#include "SkellEngine/levels/Level.hpp"

namespace engine::systems {
    GraphicalOrthogonalSystem::GraphicalOrthogonalSystem(SystemManager &manager) :
        GraphicalSystem(manager) {
    }

    GraphicalOrthogonalSystem::~GraphicalOrthogonalSystem() {

    }

    bool GraphicalOrthogonalSystem::update(sf::Int64, sf::View &) {
        // Nothing to update since animations are handled by a different system
        return true;
    }

    void GraphicalOrthogonalSystem::draw(sf::RenderTarget &target, unsigned int layer, sf::View view) {
        getSystemManager().getContext().level->drawLayer(target, layer, view);
        for (auto &entity : getEntities()) {
            auto spriteSheet = entity->getComponent("spritesheet");
            if (spriteSheet) {
                target.draw(std::static_pointer_cast<entities::components::GraphicalSpriteSheetComponent>(spriteSheet)->getSprite());
            }
        }
    }

    bool GraphicalOrthogonalSystem::checkComponents(entities::Entity::Ptr entity) const {
        return entity->hasComponent("spritesheet");
    }
}