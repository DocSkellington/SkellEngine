#pragma once

#include <SFML/Graphics.hpp>

#include "entities/components/Component.h"

namespace engine::entities::components {
    class GraphicalSpriteSheetComponent : public Component {
    public:
        explicit GraphicalSpriteSheetComponent();
        GraphicalSpriteSheetComponent(const GraphicalSpriteSheetComponent&) = delete;
        virtual ~GraphicalSpriteSheetComponent();

        virtual void create(const nlohmann::json &jsonTable) override;
        virtual void create(const sol::table &luaTable) override;
        virtual void create(const nlohmann::json &jsonTable, const sol::table &luaTable) override;

        const sf::Sprite &getSprite() const;

    private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;

        static Component::RegisterComponent<GraphicalSpriteSheetComponent> rcpc;
    };
}