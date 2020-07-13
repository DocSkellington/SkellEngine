#include "SkellEngine/entities/components/GraphicalSpriteSheetComponent.hpp"

#include "SkellEngine/files/FileManager.hpp"
#include "SkellEngine/errors/InvalidJSON.hpp"

namespace engine::entities::components {
    GraphicalSpriteSheetComponent::GraphicalSpriteSheetComponent(states::StateContext &context) :
        Component(context) {
    }

    GraphicalSpriteSheetComponent::~GraphicalSpriteSheetComponent() {

    }

    void GraphicalSpriteSheetComponent::create(const nlohmann::json &jsonTable) {
        if (auto path = jsonTable.find("filePath") ; path != jsonTable.end()) {
            if (path->is_string()) {
                sf::Texture &texture = getContext().fileManager->loadSpriteTexture(path->get<std::string>());
                m_sprite.setTexture(texture);
                m_sprite.setOrigin(texture.getSize().x / 2., texture.getSize().y / 2.);
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