#pragma once

#include <cstdint>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include "SkellEngine/map/Layers.hpp"
#include "SkellEngine/states/StateContext.hpp"

/**
 * \brief Contains the map and utilities to interact with it
 */
namespace engine::map {
    /**
     * \brief A 2D map.
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
         * \param context The context of the state
         */
        explicit Map(states::StateContext& context);
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
         * \brief Gets the properties attached to a tile's position.
         * 
         * If there are multiple tiles at that position, the different properties are fused together, i.e., a single JSON object is returned with the properties of all tiles.
         * If multiple tiles define the same property but with different values, only the value that is in the higher layer is kept.
         * \param x The x position of the tile
         * \param y The y position of the tile
         * \return A JSON object containing the properties
         */
        nlohmann::json getTileProperties(uint64_t x, uint64_t y) const;

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
         * \param target The target to draw the layer in
         * \param layer The index of the layer to draw
         * \param view The view
         * \todo Optimise according to the view
         */
        void drawLayer(sf::RenderTarget &target, std::size_t layer, sf::View view);

        /**
         * \brief Gets the number of layers
         * \return The number of layers
         */
        std::size_t getLayerCount() const;

        /**
         * \brief Gets the size of the map, in number of tiles
         * \return The number of tiles in both directions
         */
        sf::Vector2u getSizeInTiles() const;

        /**
         * \brief Gets the size of a tile
         * \return The size of a single tile
         */
        sf::Vector2u getTileSize() const;

    protected:
        /** @{ */
        /**
         * \brief Gets the state context
         * \return The state context
         */
        states::StateContext& getStateContext();
        const states::StateContext& getStateContext() const;
        /** @} */

    private:
        void loadTilesets();
        void loadTileLayer(const tmx::Layer *layer);
        void loadObjectLayer(const tmx::Layer *layer);
        void loadImageLayer(const tmx::Layer *layer);

    private:
        states::StateContext &m_context;
        const std::filesystem::path m_folder;

        tmx::Map m_map;
        std::string m_mapName;

        std::vector<std::unique_ptr<Layer>> m_layers;

        std::map<std::uint32_t, std::shared_ptr<const tmx::Tileset::Tile>> m_tilesetTiles;
        std::map<std::uint32_t, std::unique_ptr<const tmx::Vector2u>> m_tileOffset;
    };
}