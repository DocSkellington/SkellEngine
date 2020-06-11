#include "SkellEngine/entities/components/GraphicalSpriteSheetComponent.hpp"

#include "SkellEngine/files/FileManager.hpp"
#include "SkellEngine/errors/InvalidJSON.hpp"

namespace engine::entities::components {
    GraphicalSpriteSheetComponent::GraphicalSpriteSheetComponent(states::StateContext &context) :
        MemberStorage(context.context),
        Component(context) {
    }

    GraphicalSpriteSheetComponent::~GraphicalSpriteSheetComponent() {

    }

    void GraphicalSpriteSheetComponent::create(const nlohmann::json &jsonTable) {
        if (auto path = jsonTable.find("filePath") ; path != jsonTable.end()) {
            if (path->is_string()) {
                m_sprite.setTexture(getContext().fileManager->loadSpriteTexture(path->get<std::string>()));
            }
            else {
                throw errors::InvalidJSON("Sprite sheet component: invalid description: the 'filePath' field must be a string");
            }
        }
        else {
            throw errors::InvalidJSON("Sprite sheet component: invalid description: the 'filePath' field must be present");
        }
    }

    const sf::Sprite& GraphicalSpriteSheetComponent::getSprite() const {
        return m_sprite;
    }

    sf::Sprite& GraphicalSpriteSheetComponent::getSprite() {
        return m_sprite;
    }
}