#pragma once

#include <SFML/System/Time.hpp>

namespace engine::animations {
    /**
     * \brief An animation that has a duration
     * 
     * It relies on an actual animation (such as FrameAnimation)
     * \tparam The type of the animated object
     */
    template <typename Animated>
    class TimedAnimation {
    public:
        /**
         * \brief The signature the animation must have
         */
        using AnimationFunction = std::function<void(Animated&, float)>;

    public:
        /**
         * \brief Constructor
         * \param function The animation
         * \param duration The duration (in seconds) of the animation
         * \param loop Whether the animation must infinitely loop
         * \param repeats The number of times the animation must be played
         */
        TimedAnimation(AnimationFunction function, sf::Time duration, bool loop, unsigned int repeats) :
            m_animationFunction(std::move(function)),
            m_duration(duration),
            m_loop(loop),
            m_repeats(repeats) {
        }

        /**
         * \brief Gets the duration of the animation
         * \return The duration of the animation
         */
        const sf::Time& getDuration() const {
            return m_duration;
        }

        /**
         * \brief Whether this animation must loop infinitely
         * \return True iff the animation must loop infinitely
         */
        bool isLoop() const {
            return m_loop;
        }

        /**
         * \brief Gets the number of time this animation must be played
         * \return The number of time this animation must be played
         */
        unsigned int getRepeats() const {
            return m_repeats;
        }

        /**
         * \brief Applies the underlying animation
         * \param animated The animated object
         * \param progress A float between 0 and 1 indicating the progress of the animation
         */
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