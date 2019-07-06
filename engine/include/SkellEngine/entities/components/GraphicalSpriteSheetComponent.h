#pragma once

#include <SFML/Graphics.hpp>

#include "SkellEngine/entities/components/Component.h"
#include "SkellEngine/Context.h"

namespace engine::entities::components {
    /**
     * \brief Describes the sprite sheet of a component
     * \ingroup Engine
     * \todo Animations
     */
    class GraphicalSpriteSheetComponent : public Component {
    public:
        explicit GraphicalSpriteSheetComponent(Context &context);
        GraphicalSpriteSheetComponent(const GraphicalSpriteSheetComponent&) = delete;
        virtual ~GraphicalSpriteSheetComponent();

        virtual void create(const nlohmann::json &jsonTable) override;

        /**
         * \brief Returns a const reference to the sprite
         */
        const sf::Sprite &getSprite() const;

    private:
        sf::Sprite m_sprite;

        static Component::RegisterComponent<GraphicalSpriteSheetComponent> rcpc;
    };
}