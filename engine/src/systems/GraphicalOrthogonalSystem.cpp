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
        // We update the position of each sprite to reflect the actual position of the entity
        for (auto &entity : getEntities()) {
            auto spriteSheet = entity->getComponent("spritesheet");
            auto position = entity->getComponent("position");

            auto spriteSheetComponent = std::static_pointer_cast<entities::components::GraphicalSpriteSheetComponent>(spriteSheet);
            if (position) {
                // If the entity has a position component, we update the position of the Sprite
                double x = position->getFloat("x").first;
                double y = position->getFloat("y").first;
                if (position->getBool("absolute").first) {
                    // If the position is absolute, we immediately put the position of the sprite
                    spriteSheetComponent->getSprite().setPosition(x, y);
                }
                else {
                    // If the position is relative to the map, we first need to convert the level position to an absolute position
                    auto absolutePosition = getSystemManager().getContext().level->levelPositionToAbsolutePosition(x, y);
                    spriteSheetComponent->getSprite().setPosition(absolutePosition.x, absolutePosition.y);
                }
            }
        }
        return true;
    }

    void GraphicalOrthogonalSystem::draw(sf::RenderTarget &target, unsigned int layer, sf::View view) {
        getSystemManager().getContext().level->drawLayer(target, layer, view);
        for (auto &entity : getEntities()) {
            auto spriteSheet = entity->getComponent("spritesheet");
            auto spriteSheetComponent = std::static_pointer_cast<entities::components::GraphicalSpriteSheetComponent>(spriteSheet);
            target.draw(spriteSheetComponent->getSprite());
        }
    }

    bool GraphicalOrthogonalSystem::checkComponents(entities::Entity::Ptr entity) const {
        return entity->hasComponent("spritesheet");
    }
}