#pragma once

#include <cstdint>

#include <SFML/Graphics.hpp>

#include <SkellEngine/tmxlite/Map.hpp>
#include <SkellEngine/tmxlite/TileLayer.hpp>

#include "Layers.h"

namespace engine{
    struct Context;
}

/**
 * \brief Contains the map and utilities to interact with it
 */
namespace engine::map {
    /**
     * \brief A 2D map.
     * \ingroup Engine
     * 
     * The data is loaded from a Tiled file (.tmx).
     */
    class Map {
    friend class TileLayer;
    friend class ImageLayer;
    friend class ObjectLayer;
    public:
        /**
         * \brief The constructor
         * \param context The context
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
         * \brief Updates the layers
         * 
         * \param deltaTime The elapsed time since the last call frame
         */
        void updateLayers(sf::Int64 deltaTime);

        /**
         * \brief Draws a layer
         * \param window The RenderWindow
         * \param layer The index of the layer to draw
         * \param view The view
         * \todo Optimise according to the view
         */
        void drawLayer(sf::RenderWindow* window, std::size_t layer, sf::View view);

        /**
         * \brief Gets the number of layers
         * \return The number of layers
         */
        std::size_t getLayerCount() const;

    private:
        void loadTilesets();
        void loadTileLayer(const tmx::Layer *layer, const std::string& mapName);
        void loadObjectLayer(const tmx::Layer *layer, const std::string& mapName);
        void loadImageLayer(const tmx::Layer *layer, const std::string& mapName);

    private:
        Context &m_context;
        const std::string m_folder;

        tmx::Map m_map;

        std::vector<std::unique_ptr<Layer>> m_layers;

        std::map<std::uint32_t, std::shared_ptr<const tmx::Tileset::Tile>> m_tilesetTiles;
        std::map<std::uint32_t, std::unique_ptr<const tmx::Vector2u>> m_tileOffset;
    };
}