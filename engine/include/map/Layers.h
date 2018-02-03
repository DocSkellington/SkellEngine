#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ImageLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

namespace engine::map {
    class Map;

    /**
     * \brief A layer to use in a map
     * \todo Offsets
     */
    class Layer : public sf::Drawable, public sf::Transformable {
    public:
        Layer(Map &map, bool visible);
        Layer(const Layer&) = delete;
        virtual ~Layer();

        /**
         * \brief Updates the layer
         * \param deltaTime The time since the last frame
         */
        virtual void update(sf::Int32 deltaTime) = 0;

        /**
         * \brief Whether this layer should be drawn or not
         */
        bool isVisible() const;

    protected:
        Map &getMap();

    private:
        Map &m_map;
        bool m_visible;
    };

    /**
     * \brief A Layer made of tiles
     */
    class TileLayer : public Layer {
    public:
        TileLayer(Map &map, const tmx::TileLayer &layer);
        TileLayer(const TileLayer&) = delete;
        virtual ~TileLayer();

        void update(sf::Int32 deltaTime) override;

    protected:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    private:
        /**
         * \brief A Tile.
         * 
         * It handles flipping/rotation, opacity, animation.
         * A Tile knows its position
         */
        class Tile : public sf::Drawable, public sf::Transformable {
        public:
            Tile(Map &map, std::size_t x, std::size_t y, std::shared_ptr<const tmx::Tileset::Tile> tile, std::uint8_t flipFlag, std::uint8_t alpha);

            void update(sf::Int32 deltaTime);

        private:
            void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            void updateSprite();
            void flipVertical();
            void flipHorizontal();

        private:
            Map &m_map;
            std::shared_ptr<const tmx::Tileset::Tile> m_tile;
            std::uint8_t m_flip;
            std::size_t m_currentFrame;
            std::uint32_t m_elapsed;
            sf::Sprite m_sprite;
        };

    private:
        std::vector<std::vector<Tile>> tiles;
    };

    class ImageLayer : public Layer {
    public:
        ImageLayer(Map &map, const tmx::ImageLayer &layer);
        ImageLayer(const ImageLayer&) = delete;
        virtual ~ImageLayer();

        void update(sf::Int32 deltaTime) override;

    protected:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    };

    class ObjectLayer : public Layer {
    public:
        ObjectLayer(Map &map, const tmx::ObjectGroup &layer);
        ObjectLayer(const ObjectLayer&) = delete;
        virtual ~ObjectLayer();

        void update(sf::Int32 deltaTime) override;

    protected:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    };
}