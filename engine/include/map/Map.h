#pragma once

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
        class Chunk : public sf::Drawable {
        public:
            explicit Chunk();

        private:
        };

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
    
    private:
        class Tile : public sf::Drawable {
        public:
            Tile(std::string &tilesetName, sf::Vector2i& upperLeft);

        protected:
            void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            std::string &tileset;
            sf::Vector2i &ul;
        };

    private:
        void loadTilesets();
        void loadTileLayer(const tmx::Layer *layer);

    private:
        Context &m_context;
        const std::string m_folder;

        tmx::Map m_map;

        std::vector<std::vector<Tile>> m_tileLayers;

        std::vector<std::size_t> m_objectLayers;
        std::vector<std::size_t> m_imageLayers;
    };
}