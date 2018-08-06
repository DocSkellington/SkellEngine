#include "map/Layers.h"

#include <Thor/Resources.hpp>

#include <tmxlite/Log.hpp>

#include "map/Map.h"
#include "Context.h"
#include "shapes/EllipseShape.h"

namespace engine::map {
    Layer::Layer(Map &map, const std::string& mapName, bool visible) :
        m_map(map),
        m_mapName(mapName),
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

    const std::string& Layer::getMapName() const {
        return m_mapName;
    }

    TileLayer::Tile::Tile(Map &map, const std::string& mapName, std::size_t x, std::size_t y, std::shared_ptr<const tmx::Tileset::Tile> tile, std::uint8_t flipFlags, std::uint8_t alpha, const tmx::Vector2i &offset) :
        m_map(map),
        m_mapName(mapName),
        m_tile(tile),
        m_flip(flipFlags),
        m_currentFrame(0),
        m_elapsed(0) {
        m_sprite.setPosition(x * map.m_map.getTileSize().x + offset.x, y * map.m_map.getTileSize().y + offset.y);

        m_sprite.setColor(sf::Color(255, 255, 255, alpha));

        updateSprite();
    }

    void TileLayer::Tile::update(sf::Int64 deltaTime) {
        m_elapsed += deltaTime;
        while (m_elapsed >= m_tile->animation.frames[m_currentFrame].duration * 1000) {
            m_elapsed -= m_tile->animation.frames[m_currentFrame].duration * 1000;
            m_currentFrame = (m_currentFrame + 1) % m_tile->animation.frames.size();
        }

        updateSprite();
    }

    bool TileLayer::Tile::isAnimated() const {
        return m_tile->animation.frames.size() > 1;
    }

    void TileLayer::Tile::updateSprite() {
        auto tile = m_tile;
        try {
            if (m_tile->animation.frames.size() > 1) {
                auto ID = m_tile->animation.frames[m_currentFrame].tileID;
                tile = m_map.m_tilesetTiles[ID];
                m_sprite.setTexture(m_map.m_context.textureHolder->acquire(tile->imagePath, thor::Resources::fromFile<sf::Texture>(tile->imagePath), thor::Resources::Reuse));
            }
            else {
                m_sprite.setTexture(m_map.m_context.textureHolder->acquire(tile->imagePath, thor::Resources::fromFile<sf::Texture>(tile->imagePath), thor::Resources::Reuse));
            }
        }
        catch (thor::ResourceLoadingException& e) {
            tmx::Logger::logError("Error while loading a tile texture in the map " + m_mapName, e);
        }

        m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(tile->imagePosition.x, tile->imagePosition.y), sf::Vector2i(tile->imageSize.x, tile->imageSize.y)));

        m_sprite.setOrigin(tile->imageSize.x / 2.f, tile->imageSize.y / 2.f);

        m_sprite.setRotation(0.f);

        handleFlip();
    }

    void TileLayer::Tile::handleFlip() {
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

    TileLayer::TileLayer(Map &map, const std::string& mapName, const tmx::TileLayer &layer) :
        Layer(map, mapName, layer.getVisible()) {
        for (std::size_t y = 0 ; y < map.m_map.getTileCount().y ; y++) {
            std::vector<std::shared_ptr<Tile>> row;
            for (std::size_t x = 0 ; x < map.m_map.getTileCount().x ; x++) {
                std::uint8_t alpha = layer.getOpacity() * 255;

                const auto &tile = layer.getTiles()[y * map.m_map.getTileCount().y + x];

                tmx::Vector2i offset = layer.getOffset();
                offset.x += map.m_tileOffset[tile.ID-1]->x;
                offset.y += map.m_tileOffset[tile.ID-1]->y;

                std::shared_ptr<Tile> t = std::make_shared<Tile>(map, mapName, x, y, map.m_tilesetTiles[tile.ID-1], tile.flipFlags, alpha, offset);
                row.push_back(t);

                if (t->isAnimated()) {
                    animatedTiles.push_back(t);
                }
            }
            tiles.push_back(row);
        }
    }

    TileLayer::~TileLayer() {

    }

    void TileLayer::update(sf::Int64 deltaTime) {
        for (auto &tile : animatedTiles) {
            tile->update(deltaTime);
        }
    }

    void TileLayer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
        for (std::size_t y = 0 ; y < tiles.size() ; y++) {
            for (std::size_t x = 0 ; x < tiles[y].size() ; x++) {
                target.draw(*tiles[y][x], states);
            }
        }
    }

    ImageLayer::ImageLayer(Map &map, const std::string& mapName, const tmx::ImageLayer &layer) :
        Layer(map, mapName, layer.getVisible()) {
        sf::Image image;
        image.loadFromFile(layer.getImagePath());

        std::string id = layer.getImagePath();
        if (layer.hasTransparency()) {
            image.createMaskFromColor(sf::Color(layer.getTransparencyColour().r, layer.getTransparencyColour().g, layer.getTransparencyColour().b, layer.getTransparencyColour().a));
            id += std::to_string(layer.getTransparencyColour().r) + " " + std::to_string(layer.getTransparencyColour().g) + " " + std::to_string(layer.getTransparencyColour().b) + " " + std::to_string(layer.getTransparencyColour().a);
        }

        try {
            m_sprite.setTexture(getMap().m_context.textureHolder->acquire(id, thor::Resources::fromImage<sf::Texture>(image), thor::Resources::Reuse));
        }
        catch (thor::ResourceLoadingException& e) {
            tmx::Logger::logError("Error while loading an image in the map " + getMapName(), e);
        }

        m_sprite.setColor(sf::Color(255, 255, 255, layer.getOpacity() * 255));

        m_sprite.setPosition(layer.getOffset().x, layer.getOffset().y);
    }

    ImageLayer::~ImageLayer() {

    }

    void ImageLayer::update(sf::Int64 deltaTime) {
    }

    void ImageLayer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(m_sprite, states);
    }

    ObjectLayer::ObjectLayer(Map &map, const std::string& mapName, const tmx::ObjectGroup &layer) :
        Layer(map, mapName, layer.getVisible()) {
        for (const auto &object : layer.getObjects()) {
            if (object.getShape() == tmx::Object::Shape::Polyline)
                handlePolyLines(object);
            else if (object.getShape() == tmx::Object::Shape::Text)
                handleText(object);
            else
                handleShape(object);
        }
    }

    ObjectLayer::~ObjectLayer() {

    }

    void ObjectLayer::update(sf::Int64 deltaTime) {
    }

    void ObjectLayer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        for (auto &shape : m_shapes) {
            if (shape.visible)
                target.draw(*(shape.shape), states);
        }

        for (auto &line : m_lines) {
            target.draw(line, states);
        }
    }

    void ObjectLayer::handleShape(const tmx::Object &object) {
        std::unique_ptr<sf::Shape> shape;

        tmx::FloatRect AABB = object.getAABB();
        sf::Vector2f widthHeight = sf::Vector2f(AABB.width, AABB.height);

        switch (object.getShape()) {
        case tmx::Object::Shape::Rectangle:
            shape = std::make_unique<sf::RectangleShape>(widthHeight);
            break;
        case tmx::Object::Shape::Ellipse:
            shape = std::make_unique<engine::shapes::EllipseShape>(widthHeight);
            break;
        case tmx::Object::Shape::Polygon:
            shape = handlePolygon(object);
            break;
        default:
            tmx::Logger::log("Shape not implemented", tmx::Logger::Type::Warning);
            return;
        }

        shape->setPosition(sf::Vector2f(AABB.left, AABB.top));
        shape->setRotation(object.getRotation());

        for (auto &property : object.getProperties()) {
            if (property.getName() == "fill") {
                auto colour = property.getColourValue();
                shape->setFillColor(sf::Color(colour.r, colour.g, colour.b, colour.a));
            }
            else if (property.getName() == "outline") {
                auto colour = property.getColourValue();
                shape->setOutlineColor(sf::Color(colour.r, colour.g, colour.b, colour.a));
            }
            else if (property.getName() == "thickness") {
                shape->setOutlineThickness(property.getFloatValue());
            }
        }

        m_shapes.emplace_back(object.visible(), std::move(shape));
    }

    void ObjectLayer::handlePolyLines(const tmx::Object& object) {
        sf::VertexArray lines(sf::LineStrip, object.getPoints().size());

        for (std::size_t i = 0 ; i < object.getPoints().size() ; i++) {
            const auto &point = object.getPoints()[i] + object.getPosition();

            lines[i].position = sf::Vector2f(point.x, point.y);
            lines[i].color = sf::Color::White;
        }

        m_lines.push_back(lines);
    }

    std::unique_ptr<sf::Shape> ObjectLayer::handlePolygon(const tmx::Object &object) {
        std::unique_ptr<sf::ConvexShape> polygone = std::make_unique<sf::ConvexShape>(object.getPoints().size());

        for (std::size_t i = 0 ; i < object.getPoints().size() ; i++) {
            const auto &point = object.getPoints()[i];
            polygone->setPoint(i, sf::Vector2f(point.x, point.y));
        }

        return polygone;
    }

    void ObjectLayer::handleText(const tmx::Object &object) {
        // Rotation is not implemented in TGUI!

        const auto text = object.getText();

        auto label = tgui::Label::create();

        label->setPosition(object.getPosition().x, object.getPosition().y);
        label->setAutoSize(false);
        label->setSize(object.getAABB().width, object.getAABB().height);

        label->setText(text.content);

        if (text.bold) {
            label->setTextStyle(sf::Text::Bold);
        }
        if (text.italic) {
            label->setTextStyle(label->getTextStyle() | sf::Text::Italic);
        }
        if (text.underline) {
            label->setTextStyle(label->getTextStyle() | sf::Text::Underlined);
        }
        if (text.strikethough) {
            label->setTextStyle(label->getTextStyle() | sf::Text::StrikeThrough);
        }

        switch (text.hAlign) {
        case tmx::Text::HAlign::Centre:
            label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
            break;
        case tmx::Text::HAlign::Left:
            label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
            break;
        case tmx::Text::HAlign::Right:
            label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
            break;
        }

        switch (text.vAlign) {
        case tmx::Text::VAlign::Centre:
            label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
            break;
        case tmx::Text::VAlign::Bottom:
            label->setVerticalAlignment(tgui::Label::VerticalAlignment::Bottom);
            break;
        case tmx::Text::VAlign::Top:
            label->setVerticalAlignment(tgui::Label::VerticalAlignment::Top);
            break;
        }

        label->setTextColor(tgui::Color(text.colour.r, text.colour.g, text.colour.b, text.colour.a));
        label->setTextSize(text.pixelSize);

        if (text.fontFamily != "") {
            // If the font is not found, we use default one
            try {
                getMap().m_context.fileManager->loadFont(text.fontFamily);
            }
            catch (thor::ResourceLoadingException& e) {
                tmx::Logger::log("Could not load font: " + text.fontFamily + ". Default TGUI's font will be used.", tmx::Logger::Type::Warning);
            }
        }

        if (!object.visible()) {
            label->hide();
        }

        getMap().m_context.gui->add(label);
    }
}