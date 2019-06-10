#include "SkellEngine/map/Map.h"
#include "SkellEngine/Context.h"
#include "SkellEngine/errors/BadLevelDescription.h"

namespace engine::map {
    Map::Map(Context &context, const std::filesystem::path &folder) :
        m_context(context),
        m_folder(folder) {
    }

    Map::~Map() {
        
    }

    void Map::load(const std::string& mapName) {
        clear();
        std::filesystem::path mapPath = m_folder / mapName;
        mapPath += ".tmx";
        if (!m_map.load(mapPath)) {
            throw errors::BadLevelDescription("Map " + mapName + " could not be loaded. Please check that the map is in the correct folder and that the tmx file is correct.");
        }
        
        loadTilesets();

        auto &layers = m_map.getLayers();
        for (std::size_t i = 0 ; i < layers.size() ; i++) {
            const auto* layer = layers[i].get();
            switch(layer->getType()) {
            case tmx::Layer::Type::Tile:
                loadTileLayer(layer, mapName);
                break;
            case tmx::Layer::Type::Object:
                loadObjectLayer(layer, mapName);
                break;
            case tmx::Layer::Type::Image:
                loadImageLayer(layer, mapName);
                break;
            }
        }

        tmx::Logger::log("Map loaded");
    }

    void Map::clear() {
        m_layers.clear();
        m_tilesetTiles.clear();
    }

    void Map::drawLayer(sf::RenderWindow* window, std::size_t layer, sf::View view) {
        if (layer >= m_layers.size() || !m_layers[layer]->isVisible())
            return;

        window->draw(*(m_layers[layer]));
    }

    void Map::updateLayers(sf::Int64 deltaTime) {
        for (auto &layer : m_layers) {
            layer->update(deltaTime);
        }
    }

    std::size_t Map::getLayerCount() const {
        return m_layers.size();
    }

    void Map::loadTilesets() {
        for (auto &tileset : m_map.getTilesets()) {
            for (const auto &tile : tileset.getTiles()) {
                tmx::Tileset::Tile t = tile;
                t.ID = t.ID + tileset.getFirstGID() - 1;
                for (auto &anim : t.animation.frames) {
                    anim.tileID += tileset.getFirstGID() - 1;
                }
                m_tilesetTiles.emplace(t.ID, std::make_shared<tmx::Tileset::Tile>(t));
                m_tileOffset.emplace(t.ID, std::make_unique<tmx::Vector2u>(tileset.getTileOffset()));
            }
        }
    }

    void Map::loadTileLayer(const tmx::Layer *layer, const std::string &mapName) {
        const auto& l = *dynamic_cast<const tmx::TileLayer*>(layer);
        m_layers.push_back(std::make_unique<TileLayer>(*this, mapName, l));
    }

    void Map::loadObjectLayer(const tmx::Layer *layer, const std::string &mapName) {
        const auto& l = *dynamic_cast<const tmx::ObjectGroup*>(layer);
        m_layers.push_back(std::make_unique<ObjectLayer>(*this, mapName, l));
    }

    void Map::loadImageLayer(const tmx::Layer *layer, const std::string &mapName) {
        const auto& l = *dynamic_cast<const tmx::ImageLayer*>(layer);
        m_layers.push_back(std::make_unique<ImageLayer>(*this, mapName, l));
    }
}