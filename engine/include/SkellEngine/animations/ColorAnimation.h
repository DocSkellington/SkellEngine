#pragma once

#include <Thor/Graphics/ColorGradient.hpp>
#include <nlohmann/json.hpp>

namespace engine::animations {
    class ColorAnimation {
    public:
        ColorAnimation(const thor::ColorGradient &gradient);
        ColorAnimation(const nlohmann::json &description);

        template <typename Animated>
        void operator()(Animated &animated, float progress) {
            animated.setColor(m_gradient.sampleColor(progress));
        }
        
    private:
        thor::ColorGradient m_gradient;
    };
}