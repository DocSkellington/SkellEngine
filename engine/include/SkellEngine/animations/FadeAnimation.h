#pragma once

#include <nlohmann/json.hpp>
#include <SFML/Graphics/Color.hpp>

namespace engine::animations {
    /**
     * \brief An animation that applies a fading effect on an object
     * 
     * First, the animated object is completely invisible.
     * Then, the object becomes progressively opaque.
     * After that, the object starts to fade away.
     * 
     * The animated object must have a setColor(sf::Color) function.
     */
    class FadeAnimation {
    public:
        /**
         * \brief Constructor
         * \param inRatio The fraction of the animation time that is used for the fade in
         * \param outRatio The fraction of the animation time that is used for the fade out. It must be in [0, 1 - inRatio]
         * \throws std::invalid_argument if the outRatio is invalid (not in the [0, 1 - inRatio] interval)
         */
        FadeAnimation(float inRatio, float outRatio);
        /**
         * \brief Constructor
         * \param description The JSON object describing the animation
         * \throws std::invalid_argument if the outRatio is invalid (not in the [0, 1 - inRatio] interval)
         * \throws errors::InvalidJSON if the description is invalid
         */
        FadeAnimation(const nlohmann::json &description);

        /**
         * \brief Applies the fade animation of the animated object
         * \param animated The animated object
         * \param progress A float between 0 and 1 indicating the progress of the animation
         * \tparam Animated The type of the object to animate
         */
        template <typename Animated>
        void operator()(Animated &animated, float progress) {
            sf::Color color = animated.getColor();
            if (progress < m_inRatio) {
                color.a = 255 * progress / m_inRatio;
            }
            else if (progress > 1. - m_outRatio) {
                color.a = 255 * (1. - progress) / m_outRatio;
            }
            animated.setColor(color);
        }

    protected:
        /**
         * \brief Checks that the ratio is valid. That is, that outRatio is in [0, 1 - inRatio]
         * \throws std::invalid_argument if the outRatio is invalid (not in the [0, 1 - inRatio] interval)
         */
        void checkRatio() const;

    private:
        float m_inRatio;
        float m_outRatio;
    };
}