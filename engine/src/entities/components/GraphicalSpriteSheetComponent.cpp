#include "entities/components/GraphicalSpriteSheetComponent.h"

namespace engine::entities::components {
    Component::RegisterComponent<GraphicalSpriteSheetComponent> GraphicalSpriteSheetComponent::rcpc("spritesheet");

    GraphicalSpriteSheetComponent::GraphicalSpriteSheetComponent() :
        Component() {

    }

    GraphicalSpriteSheetComponent::~GraphicalSpriteSheetComponent() {

    }

    void GraphicalSpriteSheetComponent::create(Context &context, const nlohmann::json &jsonTable) {
        std::string filepath = jsonTable.at("filePath").get<std::string>();
        auto basePath = context.fileManager->getGameDescription().media.baseSprites;
        auto &texture = context.textureHolder->acquire("filePath", thor::Resources::fromFile<sf::Texture>(basePath + filepath), thor::Resources::Reuse);
        m_sprite.setTexture(texture);
    }

    void GraphicalSpriteSheetComponent::create(Context &context, const sol::table &luaTable) {

    }

    void GraphicalSpriteSheetComponent::create(Context &context, const nlohmann::json &jsonTable, const sol::table &luaTable) {

    }

    const sf::Sprite& GraphicalSpriteSheetComponent::getSprite() const {
        return m_sprite;
    }
}