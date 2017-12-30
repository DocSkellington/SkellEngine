#pragma once

#include <SFML/Graphics.hpp>

#include "entities/components/Component.h"
#include "Context.h"

namespace engine::entities::components {
    /**
     * \brief Describes the sprite sheet of a component
     * \todo Animations
     */
    class GraphicalSpriteSheetComponent : public Component {
    public:
        explicit GraphicalSpriteSheetComponent();
        GraphicalSpriteSheetComponent(const GraphicalSpriteSheetComponent&) = delete;
        virtual ~GraphicalSpriteSheetComponent();

        virtual void create(Context &context, const nlohmann::json &jsonTable) override;
        virtual void create(Context &context, const sol::table &luaTable) override;
        virtual void create(Context &context, const nlohmann::json &jsonTable, const sol::table &luaTable) override;

        /**
         * \brief Returns a const reference to the sprite
         */
        const sf::Sprite &getSprite() const;

    private:
        sf::Sprite m_sprite;

        static Component::RegisterComponent<GraphicalSpriteSheetComponent> rcpc;
    };
}