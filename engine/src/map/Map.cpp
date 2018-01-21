#include "map/Map.h"
#include "Context.h"
#include "errors/BadLevelDescription.h"

namespace engine::map {
    Map::Map(Context &context, const std::string &folder) :
        m_context(context),
        m_folder(folder) {
    }

    Map::~Map() {
        
    }

    void Map::load(const std::string& mapName) {
        clear();
        std::string mapPath = m_folder + mapName + ".tmx";
        if (!m_map.load(mapPath))
            throw errors::BadLevelDescription("Map could not be loaded. Please check that the map is in the correct folder and that the tmx file is correct.");
        
        loadTilesets();

        auto &layers = m_map.getLayers();
        for (std::size_t i = 0 ; i < layers.size() ; i++) {
            const auto* layer = layers[i].get();
            switch(layer->getType()) {
            case tmx::Layer::Type::Tile:
                loadTileLayer(layer);
                break;
            case tmx::Layer::Type::Object:
                m_objectLayers.push_back(i);
                break;
            case tmx::Layer::Type::Image:
                m_objectLayers.push_back(i);
                break;
            }
        }

        tmx::Logger::log("Map loaded");
    }

    void Map::clear() {
        m_tileLayers.clear();
    }

    void Map::loadTilesets() {
        auto &textureHolder = m_context.textureHolder;
        for (auto &tileset : m_map.getTilesets()) {
            for (auto &tile : tileset.getTiles()) {
                textureHolder->acquire(tile.imagePath, thor::Resources::fromFile<sf::Texture>(tile.imagePath), thor::Resources::Reuse);
            }
        }
    }

    void Map::loadTileLayer(const tmx::Layer *layer) {
        const auto& l = *dynamic_cast<const tmx::TileLayer*>(layer);
    }
}