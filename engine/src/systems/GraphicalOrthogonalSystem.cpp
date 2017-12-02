#include "systems/GraphicalOrthogonalSystem.h"

#include <memory>

#include "entities/components/GraphicalSpriteSheetComponent.h"

namespace engine::systems {
    System::RegisterSystem<GraphicalOrthogonalSystem> GraphicalOrthogonalSystem::rsgos("graphicalorthogonal");

    GraphicalOrthogonalSystem::GraphicalOrthogonalSystem(SystemManager &manager) :
        GraphicalSystem(manager), m_circle(100.f) {
        std::cout << "Orthogonal system init\n";
        m_circle.setFillColor(sf::Color::Red);
    }

    GraphicalOrthogonalSystem::~GraphicalOrthogonalSystem() {

    }

    bool GraphicalOrthogonalSystem::update(float deltatime) {
        m_circle.move(sf::Vector2f(deltatime, deltatime));
        // TODO
        return true;
    }

    void GraphicalOrthogonalSystem::draw(sf::RenderWindow* window, unsigned int layer) {
        window->draw(m_circle);
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