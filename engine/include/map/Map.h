#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

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
        explicit Map(const std::string &folder);
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

    private:
        const std::string &m_folder;

        tmx::Map m_map;

        std::vector<std::size_t> m_tileLayers;
        std::vector<std::size_t> m_objectLayers;
        std::vector<std::size_t> m_imageLayers;
    };
}