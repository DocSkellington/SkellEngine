#include "map/Map.h"
#include "errors/BadLevelDescription.h"

namespace engine::map {
    Map::Map(const std::string &folder) :
        m_folder(folder) {
    }

    Map::~Map() {

    }

    void Map::load(const std::string& mapName) {
        auto mapPath = m_folder + mapName + ".tmx";
        if (!m_map.load(mapPath))
            throw errors::BadLevelDescription("Map could not be loaded. Please check that the map is in the correct folder and that the tmx file is correct.");
        
        auto &layers = m_map.getLayers();
        for (std::size_t i = 0 ; i < layers.size() ; i++) {
            switch(layers[i]->getType()) {
            case tmx::Layer::Type::Tile:
                m_tileLayers.push_back(i);
                break;
            case tmx::Layer::Type::Object:
                m_objectLayers.push_back(i);
                break;
            case tmx::Layer::Type::Image:
                m_objectLayers.push_back(i);
                break;
            }
        }
    }
}