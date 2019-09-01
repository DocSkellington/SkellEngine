#include "SkellEngine/map/Map.h"

#include "SkellEngine/Context.h"
#include "SkellEngine/errors/BadLevelDescription.h"
#include "SkellEngine/errors/InvalidMap.h"
#include "SkellEngine/files/FileManager.h"
#include "SkellEngine/errors/NotImplemented.h"

namespace engine::map {
    Map::Map(states::StateContext &context) :
        m_context(context),
        m_folder(context.context.fileManager->getGameDescription().media.mapFolder) {
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
        
        m_mapName = mapName;
        loadTilesets();

        auto &layers = m_map.getLayers();
        for (std::size_t i = 0 ; i < layers.size() ; i++) {
            const auto* layer = layers[i].get();
            switch(layer->getType()) {
            case tmx::Layer::Type::Tile:
                loadTileLayer(layer);
                break;
            case tmx::Layer::Type::Object:
                loadObjectLayer(layer);
                break;
            case tmx::Layer::Type::Image:
                loadImageLayer(layer);
                break;
            default:
                throw errors::NotImplemented("Map: a map requests a not-implemented layer type");
                break;
            }
        }

        m_context.context.logger.log("Map loaded");
    }

    void Map::clear() {
        m_layers.clear();
        m_tilesetTiles.clear();
    }

    void Map::drawLayer(sf::RenderTarget &target, std::size_t layer, sf::View view) {
        if (layer >= m_layers.size() || !m_layers[layer]->isVisible())
            return;

        target.draw(*(m_layers[layer]));
    }

    void Map::updateLayers(sf::Int64 deltaTime) {
        for (auto &layer : m_layers) {
            try {
                layer->update(deltaTime);
            }
            catch (thor::ResourceLoadingException& e) {
                m_context.context.logger.logError("Error while loading a tile texture in the map " + m_mapName, e);
            }
        }
    }

    std::size_t Map::getLayerCount() const {
        return m_layers.size();
    }

    states::StateContext& Map::getStateContext() {
        return m_context;
    }

    const states::StateContext& Map::getStateContext() const {
        return m_context;
    }

    void Map::loadTilesets() {
        for (auto &tileset : m_map.getTilesets()) {
            for (const auto &tile : tileset.getTiles()) {
                tmx::Tileset::Tile t = tile;
                t.ID += tileset.getFirstGID() - 1;
                for (auto &anim : t.animation.frames) {
                    anim.tileID += tileset.getFirstGID() - 1;
                }
                m_tilesetTiles.emplace(t.ID, std::make_shared<tmx::Tileset::Tile>(t));
                m_tileOffset.emplace(t.ID, std::make_unique<tmx::Vector2u>(tileset.getTileOffset()));
            }
        }
    }

    void Map::loadTileLayer(const tmx::Layer *layer) {
        const auto& l = *dynamic_cast<const tmx::TileLayer*>(layer);
        try {
            m_layers.push_back(std::make_unique<TileLayer>(*this, l));
        }
        catch (const thor::ResourceLoadingException &e) {
            m_context.context.logger.logError("Error while loading a tile layer in the map " + m_mapName, e);
        }
        catch (const errors::InvalidMap &e) {
            m_context.context.logger.logError("Error while loading a tile layer in the map " + m_mapName, e);
        }
    }

    void Map::loadObjectLayer(const tmx::Layer *layer) {
        const auto& l = *dynamic_cast<const tmx::ObjectGroup*>(layer);
        try {
            m_layers.push_back(std::make_unique<ObjectLayer>(*this, l));
        }
        catch (const thor::ResourceLoadingException &e) {
            m_context.context.logger.logError("Error while loading a object layer in the map " + m_mapName, e);
        }
    }

    void Map::loadImageLayer(const tmx::Layer *layer) {
        const auto& l = *dynamic_cast<const tmx::ImageLayer*>(layer);
        try {
            m_layers.push_back(std::make_unique<ImageLayer>(*this, l));
        }
        catch (const thor::ResourceLoadingException& e) {
            m_context.context.logger.logError("Error while loading an image in the map " + m_mapName, e);
        }
    }
}