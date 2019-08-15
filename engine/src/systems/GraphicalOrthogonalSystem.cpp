#include "SkellEngine/systems/GraphicalOrthogonalSystem.h"

#include <memory>
#include <iostream>

#include "SkellEngine/entities/components/GraphicalSpriteSheetComponent.h"
#include "SkellEngine/levels/Level.h"

namespace engine::systems {
    GraphicalOrthogonalSystem::GraphicalOrthogonalSystem(SystemManager &manager) :
        GraphicalSystem(manager) {
    }

    GraphicalOrthogonalSystem::~GraphicalOrthogonalSystem() {

    }

    bool GraphicalOrthogonalSystem::update(sf::Int64 deltatime, sf::View &view) {
        // TODO:
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