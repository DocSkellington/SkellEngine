#pragma once

#include <cstdint>

#include <SFML/Graphics.hpp>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

namespace engine{
    struct Context;
}

/**
 * \brief Contains the map and utilities to interact with it
 */
namespace engine::map {
    class Map {
    public:
        /**
         * \brief The constructor
         * \param folder The path from "media" to the folder in which maps can be found
         */
        explicit Map(Context& context, const std::string &folder);
        Map(const Map&) = delete;
        ~Map();

        /**
         * \brief Tries to load the map of the given name.
         * 
         * The tmx file must be in the folder defined in the game.json file.
         * \param mapName The name of the map to load
         * \throw BadLevelDescription If the map could not be loaded
         */
        void load(const std::string& mapName);

        /**
         * \brief Empties the map.
         * 
         * Any references to a tile or a chunk of the map will be made caduc.
         */
        void clear();

        /**
         * \brief Updates the tiles' animations.
         * 
         * \param deltaTime The elapsed time since the last call frame
         */
        void updateTiles(sf::Int32 deltaTime);

        void drawLayer(sf::RenderWindow* window, std::size_t layer);

    private:
        class Tile : public sf::Drawable, public sf::Transformable {
        public:
            Tile(Map &map, std::size_t x, std::size_t y, std::shared_ptr<const tmx::Tileset::Tile> tile, std::uint8_t flipFlags);

            void update(sf::Int32 deltaTime);

        private:
            void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            void updateSprite();

        private:
            Map &m_map;
            std::shared_ptr<const tmx::Tileset::Tile> m_tile;
            std::uint8_t m_flip;
            std::size_t m_currentFrame;
            std::uint32_t m_elapsed;
            sf::Sprite m_sprite;
        };

        struct TileLayer : public sf::Transformable, sf::Drawable {
            std::vector<std::vector<Tile>> tiles;

        protected:
            void draw(sf::RenderTarget &target, sf::RenderStates states) const;
        };

    private:
        void loadTilesets();
        void loadTileLayer(const tmx::Layer *layer);

    private:
        Context &m_context;
        const std::string m_folder;

        tmx::Map m_map;

        std::vector<TileLayer> m_tileLayers;

        std::vector<std::size_t> m_objectLayers;
        std::vector<std::size_t> m_imageLayers;

        std::map<std::uint32_t, std::shared_ptr<const tmx::Tileset::Tile>> m_tilesetTiles;
    };
}