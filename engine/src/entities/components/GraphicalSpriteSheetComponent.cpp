#include "entities/components/GraphicalSpriteSheetComponent.h"

namespace engine::entities::components {
    Component::RegisterComponent<GraphicalSpriteSheetComponent> GraphicalSpriteSheetComponent::rcpc("spritesheet");

    GraphicalSpriteSheetComponent::GraphicalSpriteSheetComponent() :
        Component() {

    }

    GraphicalSpriteSheetComponent::~GraphicalSpriteSheetComponent() {

    }

    void GraphicalSpriteSheetComponent::create(const nlohmann::json &jsonTable) {
        std::string filepath = jsonTable.at("filePath").get<std::string>();
        auto basePath = getContext().fileManager->getGameDescription().media.baseSprites;
        auto &texture = getContext().textureHolder->acquire(filepath, thor::Resources::fromFile<sf::Texture>(basePath + filepath), thor::Resources::Reuse);
        m_sprite.setTexture(texture);
    }

    const sf::Sprite& GraphicalSpriteSheetComponent::getSprite() const {
        return m_sprite;
    }
}