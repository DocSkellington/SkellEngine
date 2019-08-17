#pragma once

#include <nlohmann/json.hpp>
#include <SFML/Graphics/Color.hpp>

namespace engine::animations {
    class FadeAnimation {
    public:
        FadeAnimation(float inRatio, float outRatio);
        FadeAnimation(const nlohmann::json &description);

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

    private:
        void checkRatio() const;

    private:
        float m_inRatio;
        float m_outRatio;
    };
}