#include "SkellEngine/shapes/EllipseShape.hpp"

#include <cmath>

namespace engine::shapes {
    EllipseShape::EllipseShape(sf::Vector2f radius) :
        m_radius(radius),
        m_pointCount(30)
    {
        update();
    }

    void EllipseShape::setRadius(sf::Vector2f radius) {
        m_radius = radius;
        update();
    }

    sf::Vector2f EllipseShape::getRadius() const {
        return m_radius;
    }

    std::size_t EllipseShape::getPointCount() const {
        return m_pointCount;
    }

    sf::Vector2f EllipseShape::getPoint(std::size_t index) const {
        static const float pi = 3.141592654f;

        float angle = index * 2 * pi / getPointCount() - pi / 2;
        float x = std::cos(angle) * m_radius.x;
        float y = std::sin(angle) * m_radius.y;

        return sf::Vector2f(m_radius.x + x, m_radius.y + y);
    }
}