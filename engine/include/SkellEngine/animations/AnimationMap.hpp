#pragma once

#include <map>

#include <SFML/System/Time.hpp>

#include "SkellEngine/animations/TimedAnimation.hpp"

/**
 * \brief Namespace for animations
 * 
 * An animation is simply a function that takes the object to animate and a float between 0 and 1 that indicates the progress of the animation.
 * 
 * Animations can be applied on every type (depending on the exact animation). They are mostly used for graphical objects.
 * 
 * The classes and the structure of this namespace are inspired from the Thor library.
 * However, this implementation allows to have animations that are used only once and then erased.
 * 
 * \see systems::AnimationSystem for a system that operates on sprites and that can be used by a state
 */
namespace engine::animations {
    /**
     * \brief A structure that stores animations. Each animation has a unique ID
     * \tparam Animated The type of the object that is animated by the animations in this structure
     * \tparam Id The type of the ID
     * \see Animator for an easy way to use animations
     */
    template <typename Animated, typename Id>
    class AnimationMap {
    public:
        /**
         * \param The type of the TimedAnimation used to store the animations
         */
        using TimedAnimationAnimated = TimedAnimation<Animated>;
        /**
         * \param The signature of the animation function
         */
        using AnimationSignature = typename TimedAnimationAnimated::AnimationFunction;

    public:
        /**
         * \brief Constructor
         */
        AnimationMap() = default;
        AnimationMap(const AnimationMap&) = delete;

        /**
         * \brief Adds a new animation
         * 
         * If there is already an animation registered for this id, the map is not modified
         * \param id The ID of the animation
         * \param animation The animation
         * \param duration The duration (in seconds) of the animation
         * \param eventToSend The event to send when the animation is finished. If the animation is played multiple times, the event is sent each time. If the string is empty, no events are sent
         * \param loop Whether the animation must infinitely loop
         * \param repeats The number of times the animation must be played
         */
        void addAnimation(Id id, AnimationSignature animation, sf::Time duration, const std::string &eventToSend = "", bool loop = false, unsigned int repeats = 1) {
            m_animations.emplace(id, TimedAnimation(animation, duration, eventToSend, loop, repeats));
        }

        /**
         * \brief Removes an animation
         * \param id The ID of the animation to remove
         */
        void removeAnimation(Id id) {
            m_animations.erase(id);
        }

        /**
         * \brief Checks if an animation is registered for the given ID
         * \param id The ID
         * \return True iff there is an animation under the given ID
         */
        bool hasAnimation(Id id) {
            return m_animations.find(id) != m_animations.end();
        }

        /** @{ */
        /**
         * \brief Gets an animation
         * \param id The ID of the animation
         * \return The animation
         * \throws std::out_of_range if the ID is not in the map
         */
        TimedAnimationAnimated& getAnimation(Id id) {
            return m_animations.at(id);
        }

        const TimedAnimationAnimated& getAnimation(Id id) const {
            return m_animations.at(id);
        }
        /** @} */

    private:
        using Container = std::map<Id, TimedAnimationAnimated>;

    private:
        Container m_animations;
    };
}