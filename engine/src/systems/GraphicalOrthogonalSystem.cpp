#include "systems/GraphicalOrthogonalSystem.h"

#include <memory>
#include <iostream>

#include "entities/components/GraphicalSpriteSheetComponent.h"

namespace engine::systems {
    System::RegisterSystem<GraphicalOrthogonalSystem> GraphicalOrthogonalSystem::rsgos("graphicalorthogonal");

    GraphicalOrthogonalSystem::GraphicalOrthogonalSystem(SystemManager &manager) :
        GraphicalSystem(manager) {
    }

    GraphicalOrthogonalSystem::~GraphicalOrthogonalSystem() {

    }

    bool GraphicalOrthogonalSystem::update(float deltatime, sf::View &view) {
        // TODO
        return true;
    }

    void GraphicalOrthogonalSystem::draw(sf::RenderWindow* window, unsigned int layer, sf::View view) {
        //getSystemManager().getContext().mapLoader->drawLayer(*window, layer);
        for (auto &entity : getEntities()) {
            auto spriteSheet = entity->getComponent("spritesheet");
            if (spriteSheet) {
                window->draw(std::static_pointer_cast<entities::components::GraphicalSpriteSheetComponent>(spriteSheet)->getSprite());
            }
        }
    }

    bool GraphicalOrthogonalSystem::checkComponents(entities::Entity::Ptr entity) const {
        return entity->hasComponent("spritesheet");
    }
}