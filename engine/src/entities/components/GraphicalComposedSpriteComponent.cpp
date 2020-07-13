#include "SkellEngine/entities/components/GraphicalComposedSpriteComponent.hpp"

#include "SkellEngine/files/FileManager.hpp"

std::map<std::string, sf::PrimitiveType> stringToType = {
    {"points", sf::PrimitiveType::Points},
    {"lines", sf::PrimitiveType::Lines},
    {"linestrip", sf::PrimitiveType::LineStrip},
    {"triangles", sf::PrimitiveType::Triangles},
    {"trianglestrip", sf::PrimitiveType::TriangleStrip},
    {"trianglefan", sf::PrimitiveType::TriangleFan},
    {"quads", sf::PrimitiveType::Quads}
};

namespace engine::entities::components {
    GraphicalComposedSpriteComponent::GraphicalComposedSpriteComponent(states::StateContext &context) :
        Component(context) {
        m_texture = nullptr;
        m_scale = {1, 1};
    }

    GraphicalComposedSpriteComponent::~GraphicalComposedSpriteComponent() {
        
    }

    void GraphicalComposedSpriteComponent::create(const nlohmann::json &jsonTable) {
        // TODO: add error handling

        if (jsonTable.is_object()) {
            if (auto type = jsonTable.find("type") ; type != jsonTable.end()) {
                if (type->is_string()) {
                    std::string t = type->get<std::string>();
                    std::transform(t.begin(), t.end(), t.begin(), [](unsigned char c) { return std::tolower(c); });
                    m_vertexArray.setPrimitiveType(stringToType[t]);
                }
            }

            if (auto texture = jsonTable.find("texture") ; texture != jsonTable.end()) {
                if (texture->is_string()) {
                    m_texture = &getContext().fileManager->loadTexture(texture->get<std::string>());
                }
            }
            else if (auto sprite = jsonTable.find("sprite") ; sprite != jsonTable.end()) {
                if (sprite->is_string()) {
                    m_texture = &getContext().fileManager->loadSpriteTexture(sprite->get<std::string>());
                }
            }

            if (auto scale = jsonTable.find("scale") ; scale != jsonTable.end()) {
                if (scale->is_number()) {
                    float s = scale->get<float>();
                    m_scale = {s, s};
                }
                else if (scale->is_array()) {
                    if (scale->size() == 2) {
                        m_scale = {(*scale)[0].get<float>(), (*scale)[1].get<float>()};
                    }
                }
            }

            // TODO: create a way to construct a color from a string
            sf::Color col = sf::Color::White;
            if (auto color = jsonTable.find("color") ; color != jsonTable.end()) {
                if (color->is_array() && color->size() == 3) {
                    col.r = (*color)[0].get<unsigned char>();
                    col.g = (*color)[1].get<unsigned char>();
                    col.b = (*color)[2].get<unsigned char>();
                }
            }

            if (auto vertices = jsonTable.find("vertices") ; vertices != jsonTable.end()) {
                if (vertices->is_array()) {
                    m_verticesRelativePositions.resize(vertices->size());
                    m_vertexArray.resize(vertices->size());
                    for (std::size_t i = 0 ; i < vertices->size() ; i++) {
                        nlohmann::json vertex = (*vertices)[i];
                        if (vertex.is_array() && (vertex.size() == 2 || vertex.size() == 5)) {
                            m_verticesRelativePositions[i].x = vertex[0].get<double>();
                            m_verticesRelativePositions[i].y = vertex[1].get<double>();
                            
                            if (m_texture) {
                                if (i == 0) {
                                    m_vertexArray[i].texCoords = sf::Vector2f(0, 0);
                                }
                                else if (i == 1) {
                                    m_vertexArray[i].texCoords = sf::Vector2f(m_texture->getSize().x, 0);
                                }
                                else if (i == 2) {
                                    m_vertexArray[i].texCoords = sf::Vector2f(m_texture->getSize().x, m_texture->getSize().y);
                                }
                                else if (i == 3) {
                                    m_vertexArray[i].texCoords = sf::Vector2f(0, m_texture->getSize().y);
                                }
                            }

                            if (vertex.size() == 5) {
                                sf::Color c;
                                c.r = vertex[2].get<unsigned char>();
                                c.g = vertex[3].get<unsigned char>();
                                c.b = vertex[4].get<unsigned char>();
                                m_vertexArray[i].color = c;
                            }
                            else {
                                m_vertexArray[i].color = col;
                            }
                        }
                    }
                }
            }
        }

        // We initialize the vertex array
        // Note that the actual position will be correctly set before this component is drawn, if the entity has a position
        updatePosition(0, 0);
    }

    void GraphicalComposedSpriteComponent::updatePosition(double x, double y) {
        for (std::size_t i = 0 ; i < m_verticesRelativePositions.size() ; i++) {
            sf::Vector2f newPosition;
            newPosition.x = x + m_verticesRelativePositions[i].x * m_scale.x;
            newPosition.y = y + m_verticesRelativePositions[i].y * m_scale.y;
            m_vertexArray[i].position = newPosition;
        }
    }

    const sf::VertexArray &GraphicalComposedSpriteComponent::getSprite() const {
        return m_vertexArray;
    }

    sf::VertexArray &GraphicalComposedSpriteComponent::getSprite() {
        return m_vertexArray;
    }

    const sf::Texture *GraphicalComposedSpriteComponent::getTexture() const {
        return m_texture;
    }
}