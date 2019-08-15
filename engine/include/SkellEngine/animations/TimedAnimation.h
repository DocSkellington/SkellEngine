#pragma once

#include <SFML/System/Time.hpp>

namespace engine::animations {
    template <typename Animated>
    class TimedAnimation {
    public:
        using AnimationFunction = std::function<void(Animated&, float)>;

    public:
        TimedAnimation(AnimationFunction function, sf::Time duration, bool loop, unsigned int repeats) :
            m_animationFunction(std::move(function)),
            m_duration(duration),
            m_loop(loop),
            m_repeats(repeats) {
        }

        const sf::Time& getDuration() const {
            return m_duration;
        }

        bool isLoop() const {
            return m_loop;
        }

        unsigned int getRepeats() const {
            return m_repeats;
        }

        void operator()(Animated &animated, float progress) {
            m_animationFunction(animated, progress);
        }

    private:
        AnimationFunction m_animationFunction;
        sf::Time m_duration;
        bool m_loop;
        unsigned int m_repeats;
    };
}