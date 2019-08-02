#pragma once

#include <SFML/Graphics/Shape.hpp>

/**
 * \brief Defines new SFML shapes
 */
namespace engine::shapes {
    /**
     * \brief Specialized shape representing a shape.
     * 
     * Mostly taken from https://www.sfml-dev.org/tutorials/2.4/graphics-shape.php
     */
    class EllipseShape final : public sf::Shape {
    public:
        /**
         * Constructor
         * \param radius The radius of the ellipse
         */
        explicit EllipseShape(sf::Vector2f radius = sf::Vector2f(0, 0));

        /**
         * \brief Change the radius of the ellipse
         * \param radius The new radius
         */
        void setRadius(sf::Vector2f radius);
        /**
         * \brief Gets the radius of the ellipse
         * \return The radius
         */
        sf::Vector2f getRadius() const;

        std::size_t getPointCount() const override;
        sf::Vector2f getPoint(std::size_t index) const override;

    private:
        sf::Vector2f m_radius;
        std::size_t m_pointCount;
    };
}