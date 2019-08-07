#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include <SkellEngine/tmxlite/Map.hpp>
#include <SkellEngine/tmxlite/TileLayer.hpp>
#include <SkellEngine/tmxlite/ImageLayer.hpp>
#include <SkellEngine/tmxlite/ObjectGroup.hpp>

namespace engine::map {
    class Map;

    /**
     * \brief A layer to use in a map.
     * 
     * For each layer type, the data is loaded from a tmx::Layer instance
     */
    class Layer : public sf::Drawable, public sf::Transformable {
    public:
        /**
         * \brief The constructor
         * \param map The map that uses this layer
         * \param mapName The name of the map
         * \param visible Whether to draw this layer
         */
        Layer(Map &map, const std::string& mapName, bool visible);
        Layer(const Layer&) = delete;
        virtual ~Layer();

        /**
         * \brief Updates the layer
         * \param deltaTime The time since the last frame
         */
        virtual void update(sf::Int64 deltaTime) = 0;

        /**
         * \brief Whether this layer should be drawn or not
         */
        bool isVisible() const;

    protected:
        /**
         * \brief Gives the map
         * \return A reference to the map
         */
        Map &getMap();
        /**
         * \brief Gives the name of the map
         * \return The name of the map
         */
        const std::string& getMapName() const;

    private:
        Map &m_map;
        const std::string& m_mapName;
        bool m_visible;
    };

    /**
     * \brief A Layer made of tiles
     */
    class TileLayer : public Layer {
    public:
        /**
         * \brief The constructor
         * \param map The map that uses this layer
         * \param mapName The name of the map
         * \param layer The description of this layer
         */
        TileLayer(Map &map, const std::string& mapName, const tmx::TileLayer &layer);
        TileLayer(const TileLayer&) = delete;
        virtual ~TileLayer();

        /**
         * \brief Updates the tile layer
         * \param deltaTime The time elapsed since the last frame
         */
        virtual void update(sf::Int64 deltaTime) override;

    protected:
        /**
         * \brief Draws the tile layer
         * \param target The render target
         * \param states The render states
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        /**
         * \brief A Tile.
         * 
         * It handles flipping/rotation, opacity, animation and offset.
         * A Tile knows its position
         */
        class Tile : public sf::Drawable, public sf::Transformable {
        public:
            /**
             * \brief The constructor
             * \param map A refernce to the map
             * \param mapName The name of the map
             * \param x The x position of the tile
             * \param y The y position of the tile
             * \param tile The tmx tile
             * \param flipFlag The flip flags (horizontal and vertical)
             * \param alpha The opacity
             * \param offset The offset from the top left corner of the layer
             */
            Tile(Map &map, const std::string& mapName, std::size_t x, std::size_t y, std::shared_ptr<const tmx::Tileset::Tile> tile, std::uint8_t flipFlag, std::uint8_t alpha, const tmx::Vector2i& offset = tmx::Vector2i(0.f, 0.f));

            /**
             * \brief Updates the tile
             * \param deltaTime The elapsed time since the last frame
             */
            void update(sf::Int64 deltaTime);

            /**
             * \brief Whether the tile is animated or not
             */
            bool isAnimated() const;

        private:
            void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
            void updateSprite();
            void handleFlip();
            void flipVertical();
            void flipHorizontal();

        private:
            Map &m_map;
            const std::string& m_mapName;
            std::shared_ptr<const tmx::Tileset::Tile> m_tile;
            std::uint8_t m_flip;
            std::size_t m_currentFrame;
            sf::Int64 m_elapsed;
            sf::Sprite m_sprite;
        };

    private:
        std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
        std::vector<std::shared_ptr<Tile>> animatedTiles;
    };

    /**
     * \brief A layer made of a single image
     */
    class ImageLayer : public Layer {
    public:
        /**
         * \brief The constructor
         * \param map The map that uses this layer
         * \param mapName The name of the map
         * \param layer The description of this layer
         */
        ImageLayer(Map &map, const std::string& mapName, const tmx::ImageLayer &layer);
        ImageLayer(const ImageLayer&) = delete;
        virtual ~ImageLayer();

        /**
         * \brief Updates the image layer
         * \param deltaTime The time elapsed since the last frame
         */
        virtual void update(sf::Int64 deltaTime) override;

    protected:
        /**
         * \brief draws the image layer
         * \param target the render target
         * \param states the render states
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::Sprite m_sprite;
    };

    /**
     * \brief A layer made of multiple shapes and texts
     */
    class ObjectLayer : public Layer {
    public:
        /**
         * \brief The constructor
         * \param map The map that uses this layer
         * \param mapName The name of the map
         * \param layer The description of this layer
         */
        ObjectLayer(Map &map, const std::string& mapName, const tmx::ObjectGroup &layer);
        ObjectLayer(const ObjectLayer&) = delete;
        virtual ~ObjectLayer();

        /**
         * \brief Updates the object layer
         * \param deltaTime The time elapsed since the last frame
         */
        virtual void update(sf::Int64 deltaTime) override;

    protected:
        /**
         * \brief draws the object layer
         * \param target the render target
         * \param states the render states
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        struct Shape {
            Shape(bool vis, std::unique_ptr<sf::Shape>s) : visible(vis), shape(std::move(s)) {}
            bool visible;
            std::unique_ptr<sf::Shape> shape;
        };

    private:
        std::vector<Shape> m_shapes;
        std::vector<sf::VertexArray> m_lines;
        std::vector<sf::Text> m_texts;

    private:
        // Every shape except lines
        void handleShape(const tmx::Object &object);
        // object must describe a polyline
        void handlePolyLines(const tmx::Object &object);
        // object must describe a polygone
        std::unique_ptr<sf::Shape> handlePolygon(const tmx::Object &object);
        // object must describe a text
        void handleText(const tmx::Object &object);
    };
}