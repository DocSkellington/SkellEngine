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

    void GraphicalSpriteSheetComponent::set(const std::string &name, long value) {
    }

    void GraphicalSpriteSheetComponent::set(const std::string &name, double value) {
    }

    void GraphicalSpriteSheetComponent::set(const std::string &name, bool value) {
    }

    void GraphicalSpriteSheetComponent::set(const std::string &name, const std::string &value) {
    }

    void GraphicalSpriteSheetComponent::set(const std::string &name, sol::nil_t value) {
    }

    void GraphicalSpriteSheetComponent::set(const std::string &name, const sol::table &value) {
    }

    void GraphicalSpriteSheetComponent::set(const std::string &name, nlohmann::json value) {
    }

    std::pair<long, bool> GraphicalSpriteSheetComponent::getInt(const std::string &name) {
    }

    std::pair<double, bool> GraphicalSpriteSheetComponent::getFloat(const std::string &name) {
    }

    std::pair<bool, bool> GraphicalSpriteSheetComponent::getBool(const std::string &name) {
    }

    std::pair<std::string, bool> GraphicalSpriteSheetComponent::getString(const std::string &name) {
    }

    std::pair<sol::object, bool> GraphicalSpriteSheetComponent::getObject(const std::string &name) {
    }
}