#include "SkellEngine/entities/components/GraphicalSpriteSheetComponent.h"

#include "SkellEngine/files/FileManager.h"

namespace engine::entities::components {
    GraphicalSpriteSheetComponent::GraphicalSpriteSheetComponent(states::StateContext &context) :
        MemberStorage(context.context),
        Component(context) {
    }

    GraphicalSpriteSheetComponent::~GraphicalSpriteSheetComponent() {

    }

    void GraphicalSpriteSheetComponent::create(const nlohmann::json &jsonTable) {
        std::string filepath = jsonTable.at("filePath").get<std::string>();
        auto &texture = getContext().fileManager->loadSpriteTexture(filepath);
        m_sprite.setTexture(texture);
    }

    const sf::Sprite& GraphicalSpriteSheetComponent::getSprite() const {
        return m_sprite;
    }

    sf::Sprite& GraphicalSpriteSheetComponent::getSprite() {
        return m_sprite;
    }
}