#pragma once

#include <SFML/Graphics.hpp>

#include "SkellEngine/entities/components/Component.h"
#include "SkellEngine/Context.h"

namespace engine::entities::components {
    /**
     * \brief Describes the sprite sheet of a component
     */
    class GraphicalSpriteSheetComponent : public Component {
    public:
        /**
         * \brief The constructor
         * \param context The context of the engine
         */
        explicit GraphicalSpriteSheetComponent(states::StateContext &context);
        GraphicalSpriteSheetComponent(const GraphicalSpriteSheetComponent&) = delete;
        virtual ~GraphicalSpriteSheetComponent();

        virtual void create(const nlohmann::json &jsonTable) override;

        /** @{ */
        /**
         * \brief Returns a const reference to the sprite
         */
        const sf::Sprite &getSprite() const;
        sf::Sprite& getSprite();
        /** @} */

    private:
        sf::Sprite m_sprite;

        REGISTER_COMPONENT(GraphicalSpriteSheetComponent, "spritesheet")
    };
}