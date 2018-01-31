#include "map/Map.h"
#include "Context.h"
#include "errors/BadLevelDescription.h"

namespace engine::map {
    Map::Tile::Tile(Map &map, std::size_t x, std::size_t y, std::shared_ptr<const tmx::Tileset::Tile> tile, std::uint8_t flipFlags) :
        m_map(map),
        m_tile(tile),
        m_flip(flipFlags),
        m_currentFrame(0),
        m_elapsed(0) {
        m_sprite.setPosition(x * map.m_map.getTileSize().x, y * map.m_map.getTileSize().y);
        updateSprite();

        auto width = m_sprite.getLocalBounds().width, height = m_sprite.getLocalBounds().height;
        m_sprite.setOrigin(width / 2.f, height / 2.f);
        m_sprite.setTextureRect(sf::IntRect(0.f, 0.f, width, height));
        m_sprite.setRotation(0.f);

        auto flip = m_flip;

        if (flip != 0)
            tmx::Logger::log("New flip");
        while (flip != 0) {
            tmx::Logger::log(std::to_string(flip));
            if (flip & tmx::TileLayer::FlipFlag::Horizontal) {
                tmx::Logger::log("Horizontal");
                flipHorizontal();
                flip -= tmx::TileLayer::FlipFlag::Horizontal;
            }
            if (flip & tmx::TileLayer::FlipFlag::Vertical) {
                tmx::Logger::log("Vertical");
                flipVertical();
                flip -= tmx::TileLayer::FlipFlag::Vertical;
            }
            if (flip & tmx::TileLayer::FlipFlag::Diagonal) {
                tmx::Logger::log("Diagonal");
                flipDiagonal();
                flip -= tmx::TileLayer::FlipFlag::Diagonal;
            }
        }
    }

    void Map::Tile::update(sf::Int64 deltaTime) {
        // We update if we only have an animation
        if (m_tile->animation.frames.size() <= 1)
            return;

        m_elapsed += deltaTime;
        while (m_elapsed >= m_tile->animation.frames[m_currentFrame].duration * 1000) {
            m_elapsed -= m_tile->animation.frames[m_currentFrame].duration * 1000;
            m_currentFrame = (m_currentFrame + 1) % m_tile->animation.frames.size();
        }

        updateSprite();

        if (m_flip != 0x0)
            tmx::Logger::log(std::to_string(static_cast<int>(m_flip)));
    }

    void Map::Tile::updateSprite() {
        if (m_tile->animation.frames.size() > 1) {
            auto ID = m_tile->animation.frames[m_currentFrame].tileID;
            auto tile = m_map.m_tilesetTiles[ID];
            m_sprite.setTexture(m_map.m_context.textureHolder->acquire(tile->imagePath, thor::Resources::fromFile<sf::Texture>(tile->imagePath), thor::Resources::Reuse));
        }
        else {
            m_sprite.setTexture(m_map.m_context.textureHolder->acquire(m_tile->imagePath, thor::Resources::fromFile<sf::Texture>(m_tile->imagePath), thor::Resources::Reuse));
        }
    }

    void Map::Tile::flipVertical() {
        auto rect = m_sprite.getTextureRect();
        m_sprite.setTextureRect(sf::IntRect(rect.left, (rect.top == 0) ? rect.height : 0, rect.width, -rect.height));
    }

    void Map::Tile::flipHorizontal() {
        auto rect = m_sprite.getTextureRect();
        m_sprite.setTextureRect(sf::IntRect((rect.left == 0) ? rect.width : 0, rect.top, -rect.width, rect.height)); 
    }

    void Map::Tile::flipDiagonal() {
        m_sprite.rotate(270.f);
        flipVertical();
    }

    void Map::Tile::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        target.draw(m_sprite);
    }

    void Map::TileLayer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        for (std::size_t y = 0 ; y < tiles.size() ; y++) {
            for (std::size_t x = 0 ; x < tiles[y].size() ; x++) {
                target.draw(tiles[y][x]);
            }
        }
    }

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
        m_tilesetTiles.clear();
    }

    void Map::drawLayer(sf::RenderWindow* window, std::size_t layer, sf::View view) {
        if (layer >= m_tileLayers.size())
            return;

        window->draw(m_tileLayers[layer]);
    }

    void Map::updateTiles(sf::Int64 deltaTime) {
        for (auto &layer : m_tileLayers) {
            for (std::size_t x = 0 ; x < m_map.getTileCount().x ; x++) {
                for (std::size_t y = 0 ; y < m_map.getTileCount().y ; y++) {
                    layer.tiles[x][y].update(deltaTime);
                }
            }
        }
    }

    void Map::loadTilesets() {
        auto &textureHolder = m_context.textureHolder;
        for (auto &tileset : m_map.getTilesets()) {
            for (const auto &tile : tileset.getTiles()) {
                m_tilesetTiles.emplace(tile.ID, std::make_shared<tmx::Tileset::Tile>(tile));
            }
        }
    }

    void Map::loadTileLayer(const tmx::Layer *layer) {
        const auto& l = *dynamic_cast<const tmx::TileLayer*>(layer);

        TileLayer la;

        for (std::size_t y = 0 ; y < m_map.getTileCount().y ; y++) {
            std::vector<Tile> row;
            for (std::size_t x = 0 ; x < m_map.getTileCount().x ; x++) {
                const auto &tile = l.getTiles()[y * m_map.getTileCount().y + x];
                Tile t(*this, x, y, m_tilesetTiles[tile.ID-1], tile.flipFlags);
                row.push_back(t);
            }
            la.tiles.push_back(row);
        }

        m_tileLayers.push_back(la);
    }
}