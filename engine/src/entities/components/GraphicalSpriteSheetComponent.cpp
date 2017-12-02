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
        std::cout << filepath << " ihgiehoi" << '\n';

        m_texture.loadFromFile("media/" + filepath);
        m_sprite.setTexture(m_texture);
    }

    void GraphicalSpriteSheetComponent::create(const sol::table &luaTable) {

    }

    void GraphicalSpriteSheetComponent::create(const nlohmann::json &jsonTable, const sol::table &luaTable) {

    }

    const sf::Sprite& GraphicalSpriteSheetComponent::getSprite() const {
        std::cout << "SPRITESAMERE\n";
        return m_sprite;
    }
}