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
        using QueueKey = long;

    public:
        Animator() :
            m_pause(false),
            m_useDefault(false),
            m_defaultAnimation(nullptr) {

        }

        void update(sf::Uint64 deltaTime) {
            if (m_useDefault && m_defaultAnimation != nullptr) {
                if (m_queues.size() == 0 || m_queues.begin()->first != 0 || m_queues.begin()->second.empty()) {
                    queue(0, *m_defaultAnimation);
                }
            }

            if (!m_pause) {
                for (auto &[key, queue] : m_queues) {
                    queue.update(deltaTime);
                }
            }
        }

        void animate(Animated &animated) {
            if (!m_pause) {
                for (auto &[key, queue] : m_queues) {
                    queue.animate(animated);
                }
            }
        }

        void enableDefault() {
            m_useDefault = true;
        }

        void disableDefault() {
            m_useDefault = false;
        }

        void setDefault(const TimedAnimationAnimated &animation) {
            m_defaultAnimation = &animation;
        }

        void play(const QueueKey &key, const TimedAnimationAnimated &animation) {
            m_pause = false;

            auto itr = m_queues.find(key);
            if (itr == m_queues.end()) {
                itr = m_queues.insert({key, Queue()}).first;
            }

            itr->second.play(animation);
        }

        void queue(const QueueKey &key, const TimedAnimationAnimated &animation) {
            auto itr = m_queues.find(key);
            if (itr == m_queues.end()) {
                itr = m_queues.insert({key, Queue()}).first;
            }

            itr->second.queue(animation);
        }

        void stop() {
            m_queues.clear();
        }

        void stop(const QueueKey &key) {
            if (auto itr = m_queues.find(key) ; itr != m_queues.end()) {
                itr->second.stop();
            }
        }

        void pause() {
            m_pause = true;
            for (auto &[key, queue] : m_queues) {
                queue.pause();
            }
        }

        void pause(const QueueKey &key) {
            if (auto itr = m_queues.find(key) ; itr != m_queues.end()) {
                itr->second.pause();
            }
        }

        void resume() {
            m_pause = false;
            for (auto [key, queue] : m_queues) {
                queue.resume();
            }
        }

        void resume(const QueueKey &key) {
            m_pause = false;
            if (auto itr = m_queues.find(key) ; itr != m_queues.end()) {
                itr->second.resume();
            }
        }

    private:
        class Queue {
        public:
            /**
             * \brief A queue is empty if there aren't any playing animations and if there aren't any queued animations
             * \return True iff the queue is empty
             */
            inline bool empty() {
                return m_queuedAnimations.size() == 0 && m_playingAnimations.size() == 0;
            }

            inline void update(sf::Uint64 deltaTime) {
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

            inline void animate(Animated &animated) {
                if (!m_pause && !m_playingAnimations.empty()) {
                    // For every playing animation except the last one, we know that the animation is fully executed
                    // So, we just call the animation function with a progress of 1
                    for (auto itr = m_playingAnimations.begin() ; itr != std::prev(m_playingAnimations.end()) ; ++itr) {
                        (*itr)(animated, 1.);
                    }

                    // For the last animation, we need to compute the exact progress
                    auto& animation = m_playingAnimations.back();
                    animation(animated, m_elapsedTime / animation.getDuration());
                }
            }

            inline void play(const TimedAnimationAnimated &animation) {
                resume();
                stop();
                m_playingAnimations.push_back(animation);
                queue(animation);
            }

            inline void queue(const TimedAnimationAnimated &animation) {
                for (unsigned int i = 0 ; i < animation.getRepeats() ; i++) {
                    m_queuedAnimations.push_back(animation);
                }
            }

            inline void stop() {
                m_elapsedTime = sf::Time::Zero;
                m_playingAnimations.clear();
                m_queuedAnimations.clear();
            }

            inline void pause() {
                m_pause = true;
            }

            inline void resume() {
                m_pause = false;
            }

        private:
            sf::Time m_elapsedTime = sf::Time::Zero;
            bool m_pause = false;
            std::list<TimedAnimationAnimated> m_playingAnimations;
            std::list<TimedAnimationAnimated> m_queuedAnimations;
        };

    private:
        bool m_pause;
        std::map<QueueKey, Queue> m_queues;
        bool m_useDefault;
        TimedAnimationAnimated const *m_defaultAnimation;
    };
}