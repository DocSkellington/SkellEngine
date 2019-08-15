#pragma once

#include <SFML/System/Time.hpp>

namespace engine::animations {
    template <typename Animated>
    class TimedAnimation {
    public:
        using AnimationFunction = std::function<void(Animated&, float)>;

    public:
        TimedAnimation(AnimationFunction function, sf::Time duration) :
            m_animationFunction(std::move(function)),
            m_duration(duration) {

        }

        const sf::Time& getDuration() const {
            return m_duration;
        }

        void operator()(Animated &animated, float progress) {
            m_animationFunction(animated, progress);
        }

    private:
        AnimationFunction m_animationFunction;
        sf::Time m_duration;
    };
}