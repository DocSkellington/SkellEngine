#pragma once

#include <list>
#include <map>

#include "SkellEngine/animations/AnimationMap.h"

namespace engine::animations {
    /**
     * \brief A structure that eases to use of animations
     * 
     * Each animation must receive a float between 0 and 1 indicating the progress of the animation.
     * The animator computes the progress of each animation.
     * 
     * The animator can also handle multiple queues of animations.
     * If animations are in different queues, they are played in parallel.
     * The queues are processed in the increasing order of their key.
     * The default queue is numbered 0.
     * Therefore, if you want to execute an animation before the default queue, use a negative key.
     * If you want to execute an animation after the default queue, use a positive key.
     * Be careful that some animations (such as FrameAnimation) overrides most values.
     * 
     * \tparam Animated The type of the object to animate
     * \see AnimationMap for an easy way to store animations
     */
    template <typename Animated>
    class Animator {
    public:
        /**
         * \brief The type of the TimedAnimation used internally
         */
        using TimedAnimationAnimated = TimedAnimation<Animated>;
        /**
         * \brief The signature of each animation
         */
        using AnimationSignature = typename TimedAnimationAnimated::AnimationFunction;
        /**
         * \brief The type of the queues' key
         */
        using QueueKey = long;

    public:
        /**
         * \brief Constructor
         */
        Animator() :
            m_pause(false),
            m_useDefault(false),
            m_defaultAnimation(nullptr) {

        }

        /**
         * \brief Updates the animator
         * 
         * Each queue is updated independtly from the others
         * 
         * If a default animation is set and if the default queue is empty, the default animation is started
         * \param deltaTime The time (in microseconds) since the last call
         * \return A list with strings. Each string is the name of an event to send. Empty strings mean no events. You can ignore this return if you do not want to handle events
         */
        std::list<std::string> update(sf::Uint64 deltaTime) {
            if (m_useDefault && m_defaultAnimation != nullptr) {
                if (m_queues.size() == 0 || m_queues.begin()->first != 0 || m_queues.begin()->second.empty()) {
                    queue(0, *m_defaultAnimation);
                }
            }

            std::list<std::string> events;

            if (!m_pause) {
                for (auto &[key, queue] : m_queues) {
                    auto e = queue.update(deltaTime);
                    std::move(e.begin(), e.end(), std::back_inserter(events));
                }
            }

            return events;
        }

        /**
         * \brief Animated an object
         * 
         * Applies the playing animations in each queue on the object to animate
         * \param animated The object to animate
         */
        void animate(Animated &animated) {
            if (!m_pause) {
                for (auto &[key, queue] : m_queues) {
                    queue.animate(animated);
                }
            }
        }

        /**
         * \brief Enables the use of a default animation
         */
        void enableDefault() {
            m_useDefault = true;
        }

        /**
         * \brief Disables the use of a default animation
         */
        void disableDefault() {
            m_useDefault = false;
        }

        /**
         * \brief Sets the default animation
         * 
         * The animation must exist as long as it's used.
         * If the animation is destroyed and the default animation is used, the program will crash.
         * \param animation The animation
         */
        void setDefault(const TimedAnimationAnimated &animation) {
            m_defaultAnimation = &animation;
        }

        /**
         * \brief Clears the queue with the given key, resumes the animator and adds the animation in the queue
         * \param key The key of the queue
         * \param animation The animaiton to play
         */
        void play(const QueueKey &key, const TimedAnimationAnimated &animation) {
            m_pause = false;

            auto itr = m_queues.find(key);
            if (itr == m_queues.end()) {
                itr = m_queues.insert({key, Queue()}).first;
            }

            itr->second.play(animation);
        }

        /**
         * \brief Adds the animation in the queue of the given key without clearing the queue and whithout resuming the animator
         * \param key The key of the queue
         * \param animation The animation
         */
        void queue(const QueueKey &key, const TimedAnimationAnimated &animation) {
            auto itr = m_queues.find(key);
            if (itr == m_queues.end()) {
                itr = m_queues.insert({key, Queue()}).first;
            }

            itr->second.queue(animation);
        }

        /**
         * \brief Clears every queue
         */
        void stop() {
            m_queues.clear();
        }

        /**
         * \brief Clears the queue of the given key
         * \param key The key of the queue to clear
         */
        void stop(const QueueKey &key) {
            if (auto itr = m_queues.find(key) ; itr != m_queues.end()) {
                itr->second.stop();
            }
        }

        /**
         * \brief Pauses the animator and every queue
         */
        void pause() {
            m_pause = true;
            for (auto &[key, queue] : m_queues) {
                queue.pause();
            }
        }

        /**
         * \brief Pauses the queue with the given key. It doesn't pause the animator itself
         * \param key The key of the queue to pause
         */
        void pause(const QueueKey &key) {
            if (auto itr = m_queues.find(key) ; itr != m_queues.end()) {
                itr->second.pause();
            }
        }

        /**
         * \brief Resumes the animator and every queue
         */
        void resume() {
            m_pause = false;
            for (auto [key, queue] : m_queues) {
                queue.resume();
            }
        }

        /**
         * \brief Resumes the animator and the queue with the given key
         * \param key The key of the queue to resume
         */
        void resume(const QueueKey &key) {
            m_pause = false;
            if (auto itr = m_queues.find(key) ; itr != m_queues.end()) {
                itr->second.resume();
            }
        }

    private:
        /**
         * \brief A queue
         * 
         * It stores playing animations, queued animations, the elapsed time and whether the queue is paused
         */
        class Queue {
        public:
            /**
             * \brief A queue is empty if there aren't any playing animations and if there aren't any queued animations
             * \return True iff the queue is empty
             */
            inline bool empty() {
                return m_queuedAnimations.size() == 0 && m_playingAnimations.size() == 0;
            }

            inline std::list<std::string> update(sf::Uint64 deltaTime) {
                std::list<std::string> eventsToSend;

                if (!m_pause) {
                    // How much time we have left to execute
                    sf::Time durationLeft = sf::Time::Zero;

                    // First, we remove the completely played animations from the list
                    // We also throw the events, if needed
                    if (!m_playingAnimations.empty()) {
                        typename std::list<TimedAnimationAnimated>::iterator endIterator;
                        // Since it's a queue, we know that the last animation in the queue is the last played animation
                        sf::Time lastAnimationDuration = m_playingAnimations.back().getDuration();
                        if (m_elapsedTime < lastAnimationDuration) {
                            // Every animation is finished except the last one
                            durationLeft = lastAnimationDuration;
                            endIterator = std::prev(m_playingAnimations.end());
                        }
                        else {
                            // The last animation was fully executed
                            endIterator = m_playingAnimations.end();
                        }
    
                        // We effectively remove the animations and store the events to send
                        for (auto itr = m_playingAnimations.begin() ; itr != endIterator ; ) {
                            eventsToSend.push_back(itr->getEventToSend());
                            itr = m_playingAnimations.erase(itr);
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

                return eventsToSend;
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