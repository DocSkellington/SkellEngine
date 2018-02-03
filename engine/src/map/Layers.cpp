#include "map/Layers.h"

#include <Thor/Resources.hpp>

#include <tmxlite/detail/Log.hpp>
#include "map/Map.h"
#include "Context.h"

namespace engine::map {
    Layer::Layer(Map &map, bool visible) :
        m_map(map),
        m_visible(visible) {
    }

    Layer::~Layer() {

    }

    bool Layer::isVisible() const {
        return m_visible;
    }

    Map& Layer::getMap() {
        return m_map;
    }

    TileLayer::Tile::Tile(Map &map, std::size_t x, std::size_t y, std::shared_ptr<const tmx::Tileset::Tile> tile, std::uint8_t flipFlags, std::uint8_t alpha) :
        m_map(map),
        m_tile(tile),
        m_flip(flipFlags),
        m_currentFrame(0),
        m_elapsed(0) {
        m_sprite.setPosition(x * map.m_map.getTileSize().x, y * map.m_map.getTileSize().y);
        m_sprite.setColor(sf::Color(255, 255, 255, alpha));
        updateSprite();
    }

    void TileLayer::Tile::update(sf::Int32 deltaTime) {
        // We update if we only have an animation
        if (m_tile->animation.frames.size() <= 1)
            return;

        m_elapsed += deltaTime;
        while (m_elapsed >= m_tile->animation.frames[m_currentFrame].duration) {
            m_elapsed -= m_tile->animation.frames[m_currentFrame].duration;
            m_currentFrame = (m_currentFrame + 1) % m_tile->animation.frames.size();
        }

        updateSprite();
    }

    void TileLayer::Tile::updateSprite() {
        if (m_tile->animation.frames.size() > 1) {
            auto ID = m_tile->animation.frames[m_currentFrame].tileID;
            auto tile = m_map.m_tilesetTiles[ID];
            m_sprite.setTexture(m_map.m_context.textureHolder->acquire(tile->imagePath, thor::Resources::fromFile<sf::Texture>(tile->imagePath), thor::Resources::Reuse));
        }
        else {
            m_sprite.setTexture(m_map.m_context.textureHolder->acquire(m_tile->imagePath, thor::Resources::fromFile<sf::Texture>(m_tile->imagePath), thor::Resources::Reuse));
        }

        auto width = m_sprite.getLocalBounds().width, height = m_sprite.getLocalBounds().height;
        m_sprite.setOrigin(width / 2.f, height / 2.f);
        m_sprite.setTextureRect(sf::IntRect(0.f, 0.f, width, height));
        m_sprite.setRotation(0.f);

        if (m_flip & tmx::TileLayer::FlipFlag::Horizontal) {
            if (m_flip & tmx::TileLayer::FlipFlag::Vertical) {
                if (m_flip & tmx::TileLayer::FlipFlag::Diagonal) {
                    // Horizontal + Vertical + Diagonal
                    m_sprite.rotate(270.f);
                    flipVertical();
                }
                else {
                    // Horizontal + Vertical
                    m_sprite.rotate(180.f);
                }
            }
            else {
                if (m_flip & tmx::TileLayer::FlipFlag::Diagonal) {
                    // Horizontal + Diagonal
                    m_sprite.rotate(90.f);
                }
                else {
                    // Horizontal
                    flipHorizontal();
                }
            }
        }
        else {
            if (m_flip & tmx::TileLayer::FlipFlag::Vertical) {
                if (m_flip & tmx::TileLayer::FlipFlag::Diagonal) {
                    // Vertical + Diagonal
                    m_sprite.rotate(270.f);
                }
                else {
                    // Vertical
                    flipVertical();
                }
            }
            else {
                if (m_flip & tmx::TileLayer::FlipFlag::Diagonal) {
                    // Diagonal
                    m_sprite.rotate(90.f);
                    flipVertical();
                }
            }
        }
    }

    void TileLayer::Tile::flipVertical() {
        auto rect = m_sprite.getTextureRect();
        m_sprite.setTextureRect(sf::IntRect(rect.left, (rect.top == 0) ? rect.height : 0, rect.width, -rect.height));
    }

    void TileLayer::Tile::flipHorizontal() {
        auto rect = m_sprite.getTextureRect();
        m_sprite.setTextureRect(sf::IntRect((rect.left == 0) ? rect.width : 0, rect.top, -rect.width, rect.height)); 
    }

    void TileLayer::Tile::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        target.draw(m_sprite);
    }

    TileLayer::TileLayer(Map &map, const tmx::TileLayer &layer) :
        Layer(map, layer.getVisible()) {
        for (std::size_t y = 0 ; y < map.m_map.getTileCount().y ; y++) {
            std::vector<Tile> row;
            for (std::size_t x = 0 ; x < map.m_map.getTileCount().x ; x++) {
                std::uint8_t alpha = layer.getOpacity() * 255;
                const auto &tile = layer.getTiles()[y * map.m_map.getTileCount().y + x];
                Tile t(map, x, y, map.m_tilesetTiles[tile.ID-1], tile.flipFlags, alpha);
                row.push_back(t);
            }
            tiles.push_back(row);
        }
    }

    TileLayer::~TileLayer() {

    }

    void TileLayer::update(sf::Int32 deltaTime) {
        for (std::size_t x = 0 ; x < tiles.size() ; x++) {
            for (std::size_t y = 0 ; y < tiles[x].size() ; y++) {
                tiles[x][y].update(deltaTime);
            }
        }
    }

    void TileLayer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        for (std::size_t y = 0 ; y < tiles.size() ; y++) {
            for (std::size_t x = 0 ; x < tiles[y].size() ; x++) {
                target.draw(tiles[y][x]);
            }
        }
    }

    ImageLayer::ImageLayer(Map &map, const tmx::ImageLayer &layer) :
        Layer(map, layer.getVisible()) {

    }

    ImageLayer::~ImageLayer() {

    }

    void ImageLayer::update(sf::Int32 deltaTime) {
        /**
         * \todo Implement
         */
    }

    void ImageLayer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        /**
         * \todo Implement
         */
    }

    ObjectLayer::ObjectLayer(Map &map, const tmx::ObjectGroup &layer) :
        Layer(map, layer.getVisible()) {

    }

    ObjectLayer::~ObjectLayer() {

    }

    void ObjectLayer::update(sf::Int32 deltaTime) {
        /**
         * \todo Implement
         */
    }

    void ObjectLayer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        /**
         * \todo Implement
         */
    }
}