#pragma once

#include <map>

#include <SFML/System/Time.hpp>

#include "SkellEngine/animations/TimedAnimation.h"

/**
 * \brief Namespace for animations
 * 
 * Animations can be applied on always every type (depending on the exact animation). They are mostly used for graphical objects.
 * 
 * The classes are inspired from the Thor library. However, this implementation adds more animation types and allows to have animations that are used only once and then erased.
 */
namespace engine::animations {
    template <typename Animated, typename Id>
    class AnimationMap {
    public:
        using TimedAnimationAnimated = TimedAnimation<Animated>;
        using AnimationSignature = typename TimedAnimationAnimated::AnimationFunction;

    public:
        AnimationMap() {

        }

        void addAnimation(Id id, AnimationSignature animation, sf::Time duration, bool loop = false, unsigned int repeats = 1) {
            m_animations.emplace(id, TimedAnimation(animation, duration, loop, repeats));
        }

        void removeAnimation(Id id) {
            m_animations.erase(id);
        }

        TimedAnimationAnimated& getAnimation(Id id) {
            return m_animations.at(id);
        }

        const TimedAnimationAnimated& getAnimation(Id id) const {
            return m_animations.at(id);
        }

    private:
        using Container = std::map<Id, TimedAnimationAnimated>;

    private:
        Container m_animations;
    };
}