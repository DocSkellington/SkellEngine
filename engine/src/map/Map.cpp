#include "map/Map.h"
#include <cstdlib>
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
                loadObjectLayer(layer);                
                break;
            case tmx::Layer::Type::Image:
                loadImageLayer(layer);
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

    void Map::updateLayers(sf::Int32 deltaTime) {
        for (auto &layer : m_layers) {
            layer->update(deltaTime);
        }
    }

    void Map::loadTilesets() {
        for (auto &tileset : m_map.getTilesets()) {
            tmx::Logger::log(std::to_string(tileset.getTiles().size()));
            if (tileset.getTiles().size() > 0) {
                // Collection of images
                for (const auto &tile : tileset.getTiles()) {
                    tmx::Logger::log(std::to_string(tile.imageSize.x));
                    tmx::Logger::log(std::to_string(tile.imageSize.y));
                    m_tilesetTiles.emplace(tile.ID, std::make_shared<tmx::Tileset::Tile>(tile));
                    m_tileOffset.emplace(tile.ID, std::make_unique<tmx::Vector2u>(tileset.getTileOffset()));
                }
            }
            else {
                tmx::Logger::log("Image tileset is not supported... The game will close", tmx::Logger::Type::Error);
                exit(EXIT_FAILURE);
                // Tileset Image
                /*std::uint32_t rowCount = std::floor(tileset.getTileCount() / tileset.getColumnCount());
                auto firstID = tileset.getFirstGID();
                auto width = tileset.getTileSize().x, height = tileset.getTileSize().y;

                for (std::size_t row = 0 ; row < rowCount ; row++) {
                    for (std::size_t column = 0 ; column < tileset.getColumnCount() ; column++) {
                        tmx::Vector2u imagePosition;
                        imagePosition.x = column * width;
                        if (column > 0)
                            imagePosition.x += (column - 1) * tileset.getSpacing();
                        imagePosition.y = row * height;
                        if (row > 0)
                            imagePosition.y += (row - 1) * tileset.getSpacing();

                        tmx::Vector2u imageSize(width, height);

                        tmx::Tileset::Tile tile;
                        tile.imagePath = tileset.getImagePath();
                        tile.imagePosition = imagePosition;
                        tile.imageSize = imageSize;
                        tile.properties = tileset.getProperties();
                    }
                }
                */
            }
            tmx::Logger::log("Tileset");
        }
    }

    void Map::loadTileLayer(const tmx::Layer *layer) {
        const auto& l = *dynamic_cast<const tmx::TileLayer*>(layer);
        m_layers.push_back(std::make_unique<TileLayer>(*this, l));
    }

    void Map::loadObjectLayer(const tmx::Layer *layer) {
        const auto& l = *dynamic_cast<const tmx::ObjectGroup*>(layer);
        m_layers.push_back(std::make_unique<ObjectLayer>(*this, l));
    }

    void Map::loadImageLayer(const tmx::Layer *layer) {
        const auto& l = *dynamic_cast<const tmx::ImageLayer*>(layer);
        m_layers.push_back(std::make_unique<ImageLayer>(*this, l));
    }
}