#pragma once

#include <queue>
#include <list>

#include "SkellEngine/animations/AnimationMap.h"

namespace engine::animations {
    template <typename Animated, typename Id>
    class Animator {
    public:
        using AnimationMap = AnimationMap<Animated, Id>;
        using AnimationSignature = typename AnimationMap::AnimationSignature;
        using TimedAnimation = typename AnimationMap::TimedAnimation;

    public:
        Animator(AnimationMap &map) :
            m_animationsMap(map),
            m_pause(false),
            m_elapsedTime(sf::Time::Zero) {

        }

        void update(sf::Uint64 deltaTime) {
            // TODO: handle when the animation is deleted from the map
            // We add the deltaTime to the elapsed time of the animator
            m_elapsedTime += sf::microseconds(deltaTime);

            // How much time we have left to execute
            sf::Time duration = sf::Time::Zero;

            // First, we clear the list of playing animations
            if (!m_playingAnimations.empty()) {
                // Since it's a queue, we know that the last animation in the queue is the last played animation
                sf::Time lastAnimationDuration = m_animationsMap.getAnimation(m_playingAnimations.back()).getDuration();
                if (lastAnimationDuration < m_elapsedTime) {
                    // We stille have to update the last animation. Therefore, we remove everything except the last one
                    m_playingAnimations.erase(m_playingAnimations.begin(), std::prev(m_playingAnimations.end()));
                    duration = lastAnimationDuration;
                }
                else {
                    // The last animation was fully executed. We can remove everything
                    m_playingAnimations.clear();
                }
            }

            // The last played animation lefts enough time to start a new animation
            while (m_elapsedTime >= duration) {
                // TODO: once the queue is implemented
                m_elapsedTime = duration;
                break;
            }
        }

        void animate(Animated &animated) {
            if (!m_playingAnimations.empty()) {
                // For every playing animation except the last one, we know that the animation is fully executed
                // So, we just call the animation function with a progress of 1
                for (auto itr = m_playingAnimations.begin() ; itr != std::prev(m_playingAnimations.end()) ; ++itr) {
                    m_animationsMap.getAnimation(*itr)(animated, 1.);
                }

                // For the last animation, we need to compute the exact progress
                auto animation = m_animationsMap.getAnimation(*m_playingAnimations.end());
                animation(animated, m_elapsedTime / animation.getDuration());
            }
        }

        void play() {
            m_pause = false;
        }

        void play(const std::string &animation) {
            stop();
            m_playingAnimations.push_back(animation);
        }

        void queue(const std::string &animation) {

        }

        void stop() {
            m_elapsedTime = sf::Time::Zero;
            m_playingAnimations.clear();
        }

        void pause() {
            m_pause = true;
        }

        void togglePause() {
            m_pause = !m_pause;
        }

    private:
        AnimationMap& m_animationsMap;
        bool m_pause;
        sf::Time m_elapsedTime;
        std::list<std::string> m_playingAnimations;
        // TODO: queue waiting animations
    };
}