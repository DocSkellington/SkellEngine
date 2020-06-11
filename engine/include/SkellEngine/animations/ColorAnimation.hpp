#pragma once

#include <Thor/Graphics/ColorGradient.hpp>
#include <nlohmann/json.hpp>

namespace engine::animations {
    /**
     * \brief An animation that gradually changes the color of an object over time
     * 
     * The animated object must have a setColor(sf::Color) function
     */
    class ColorAnimation {
    public:
        /**
         * \brief Constructor
         * \param gradient The gradient to use
         */
        ColorAnimation(const thor::ColorGradient &gradient);
        /**
         * \brief Constructor
         * \param description The JSON object that contains the description of the gradient
         * \throws std::invalid_argument if the description is invalid
         */
        ColorAnimation(const nlohmann::json &description);

        /**
         * \brief Applies the color animation of the animated object
         * \param animated The animated object
         * \param progress A float between 0 and 1 indicating the progress of the animation
         * \tparam Animated The type of the object to animate
         */
        template <typename Animated>
        void operator()(Animated &animated, float progress) {
            animated.setColor(m_gradient.sampleColor(progress));
        }
        
    private:
        thor::ColorGradient m_gradient;
    };
}