#pragma once

#include <list>
#include <iostream>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <nlohmann/json.hpp>

namespace engine::animations {
    /**
     * \brief An animation that works frame by frame
     * 
     * Each frame has a certain duration (relative to the duration of the animation) and can modify the scale, color, texture rect, origin and rotation of an animated object
     * 
     * The animated object must have the following functions:
     *      - setTextureRect(sf::IntRect)
     *      - setOrigin(sf::Vector2f)
     *      - scale(sf::Vector2f)
     *      - setScale(sf::Vector2f)
     *      - rotate(float)
     *      - setRotation(float)
     *      - setColor(sf::Color)
     */
    class FrameAnimation {
    public:
        /**
         * \brief Constructor
         * 
         * The animation does not contain any frame
         */
        FrameAnimation();
        /**
         * \brief Constructor
         * \param description The description of the frame animation
         * \throws errors::InvalidJSON if the description is invalid
         */
        FrameAnimation(const nlohmann::json &description);
        /**
         * \brief Copy constructor
         * \param frameAnimation The frame animation to copy
         */
        FrameAnimation(const FrameAnimation &frameAnimation);

        /**
         * \brief Adds a frame in the animation
         * 
         * The frame is always added after the already defined frames
         * 
         * If the sum of the relative frames before this new frame exceeds 1, the new frame is never executed
         */
        void addFrame(const nlohmann::json &description);

        /**
         * \brief Applies the frame animation of the animated object
         * \param animated The animated object
         * \param progress A float between 0 and 1 indicating the progress of the animation
         * \tparam Animated The type of the object to animate
         */
        template <typename Animated>
        void operator()(Animated &animated, float progress) {
            float totalProgress = 0.f;
            for (auto &frame : m_frames) {
                if (totalProgress <= progress && progress < totalProgress + frame.relativeDuration) {
                    frame(animated);
                    break;
                }
                totalProgress += frame.relativeDuration;
            }
        }

    protected:
        /**
         * \brief Stores the data for each frame
         */
        struct Frame {
            /**
             * \brief Applies the frame of the animated object
             * \param animated The animated object
             * \tparam Animated The type of the object to animate
             */
            template <typename Animated>
            void operator() (Animated &animated) {
                if (applyTextureRect) {
                    animated.setTextureRect(textureRect);
                }
                if (applyOrigin) {
                    animated.setOrigin(origin);
                }

                if (applyScale && !scale.applied) {
                    if (scale.relative) {
                        animated.scale(scale.data);
                        scale.applied = true;
                    }
                    else {
                        animated.setScale(scale.data);
                    }
                }

                if (applyRotation && !rotation.applied) {
                    if (rotation.relative) {
                        animated.rotate(rotation.data);
                        rotation.applied = true;
                    }
                    else {
                        animated.setRotation(rotation.data);
                    }
                }

                if (applyColor) {
                    animated.setColor(color);
                }
            }

            /**
             * \brief Stores a value and whether the value if relative of absolute
             * \tparam T The type of the value to store
             */
            template<typename T>
            struct RelativeOrAbsolute {
                /**
                 * \brief The value
                 */
                T data;
                /**
                 * \brief Whether the value must be considered as relative or absolute
                 */
                bool relative;
                /**
                 * \brief For relative values, we want to apply the modification exactly once. Therefore, we must remember if we've already applied the modification
                 * 
                 * This value is reset to false when the animation is copied
                 */
                bool applied = false;
            };

            /**
             * \brief The texture rect for this frame
             */
            sf::IntRect textureRect;
            /**
             * \brief Whether to apply the texture rect
             */
            bool applyTextureRect = false;
            /**
             * \brief The origin for this frame
             */
            sf::Vector2f origin;
            /**
             * \brief Whether to apply the origin
             */
            bool applyOrigin = false;
            /**
             * \brief The scale for this frame
             */
            RelativeOrAbsolute<sf::Vector2f> scale;
            /**
             * \brief Whether to apply the scale
             */
            bool applyScale = false;
            /**
             * \brief The rotation for this frame
             */
            RelativeOrAbsolute<float> rotation;
            /**
             * \brief Whether to apply the rotation
             */
            bool applyRotation = false;
            /**
             * \brief The color for this frame
             */
            sf::Color color;
            /**
             * \brief Whether to apply the color
             */
            bool applyColor = false;
            /**
             * \brief The duration of this frame
             */
            float relativeDuration;
        };

    private:
        std::list<Frame> m_frames;
    };
}