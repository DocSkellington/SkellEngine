#pragma once

#include <list>
#include <map>

#include "SkellEngine/animations/AnimationMap.h"

namespace engine::animations {
    template <typename Animated>
    class Animator {
    public:
        using TimedAnimationAnimated = TimedAnimation<Animated>;
        using AnimationSignature = typename TimedAnimationAnimated::AnimationFunction;

    public:
        Animator() :
            m_pause(false),
            m_elapsedTime(sf::Time::Zero) {

        }

        void update(sf::Uint64 deltaTime) {
            if (!m_pause) {
                // How much time we have left to execute
                sf::Time durationLeft = sf::Time::Zero;

                // First, we clear the list of playing animations
                if (!m_playingAnimations.empty()) {
                    // Since it's a queue, we know that the last animation in the queue is the last played animation
                    sf::Time lastAnimationDuration = m_playingAnimations.back().getDuration();
                    if (m_elapsedTime < lastAnimationDuration) {
                        // We stille have to update the last animation. Therefore, we remove everything except the last one
                        m_playingAnimations.erase(m_playingAnimations.begin(), std::prev(m_playingAnimations.end()));
                        durationLeft = lastAnimationDuration;
                    }
                    else {
                        // The last animation was fully executed. We can remove everything
                        m_playingAnimations.clear();
                    }
                }

                // We add the deltaTime to the elapsed time of the animator
                m_elapsedTime += sf::microseconds(deltaTime);

                // The last played animation lefts enough time to start a new animation
                while (m_elapsedTime >= durationLeft) {
                    // If the last animation must loop, we add it at the first position in the queued animations
                    if (!m_playingAnimations.empty() && m_playingAnimations.back().isLoop()) {
                        m_queuedAnimations.push_front(m_playingAnimations.back());
                    }

                    if (m_queuedAnimations.empty()) {
                        // We can't directly set to Time::Zero because of the animate function that uses m_elapsedTime
                        // So, we set the variable to the left duration
                        // If there is no animation at all for a frame, m_elapsedTime goes back to Time::Zero
                        m_elapsedTime = durationLeft;
                        break;
                    }
                    else {
                        // We retrieve the next animation and start to play it
                        auto nextAnimation = m_queuedAnimations.front();
                        m_queuedAnimations.pop_front();
                        m_playingAnimations.push_back(nextAnimation);

                        // We update duration and the elapsed time to match the new animation
                        m_elapsedTime -= durationLeft; // We remove the time it took until here
                        durationLeft = nextAnimation.getDuration();
                    }
                }
            }
        }

        void animate(Animated &animated) {
            if (!m_playingAnimations.empty()) {
                // For every playing animation except the last one, we know that the animation is fully executed
                // So, we just call the animation function with a progress of 1
                for (auto itr = m_playingAnimations.begin() ; itr != std::prev(m_playingAnimations.end()) ; ++itr) {
                    (*itr)(animated, 1.);
                }

                // For the last animation, we need to compute the exact progress
                auto animation = m_playingAnimations.back();
                animation(animated, m_elapsedTime / animation.getDuration());
            }
        }

        void play(const TimedAnimationAnimated &animation) {
            m_pause = false;
            stop();

            m_playingAnimations.push_back(animation);
            for (unsigned int i = 1 ; i < animation.getRepeats() ; i++) {
                m_queuedAnimations.push_back(animation);
            }
            std::cout << m_playingAnimations.size() << "\n";
        }

        void queue(const TimedAnimationAnimated &animation) {
            for (unsigned int i = 0 ; i < animation.getRepeats() ; i++) {
                m_queuedAnimations.push_back(animation);
            }
        }

        void stop() {
            m_elapsedTime = sf::Time::Zero;
            m_playingAnimations.clear();
        }

        void pause() {
            m_pause = true;
        }

        void resume() {
            m_pause = false;
        }

        void togglePause() {
            m_pause = !m_pause;
        }

    private:
        bool m_pause;
        sf::Time m_elapsedTime;
        std::list<TimedAnimationAnimated> m_playingAnimations;
        std::list<TimedAnimationAnimated> m_queuedAnimations;
    };
}