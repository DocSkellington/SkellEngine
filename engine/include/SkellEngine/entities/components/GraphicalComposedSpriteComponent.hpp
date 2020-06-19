#pragma once

#include <SFML/Graphics.hpp>

#include "SkellEngine/entities/components/Component.hpp"

namespace engine::entities::components {
    /**
     * \brief A graphical sprite described by its vertices.
     * 
     * A "composed sprite" is described by vertices.
     * That is, the user provides the position of each vertex relative to the center of the object (0, 0).
     * 
     * Note that only one texture file can be loaded.
     */
    class GraphicalComposedSpriteComponent : public Component {
    public:
        explicit GraphicalComposedSpriteComponent(states::StateContext &context);
        GraphicalComposedSpriteComponent(const GraphicalComposedSpriteComponent&) = delete;
        virtual ~GraphicalComposedSpriteComponent();

        virtual void create(const nlohmann::json &jsonTable) override;

        /**
         * \brief Updates the position of the vertices.
         * 
         * Each vertex's position is given by (x + relative position * scale, y + relative position * scale).
         * \param x The x position of the center of the object.
         * \param y The y position of the center of the object.
         */
        void updatePosition(double x, double y);

        const sf::VertexArray &getSprite() const;
        sf::VertexArray &getSprite();

        const sf::Texture *getTexture() const;

    private:
        sf::VertexArray m_vertexArray;
        std::vector<sf::Vector2f> m_verticesRelativePositions;
        sf::Texture *m_texture;
        sf::Vector2f m_scale;

        REGISTER_COMPONENT(GraphicalComposedSpriteComponent, "composedsprite")
    };
}