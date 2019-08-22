#pragma once

#include <list>
#include <iostream>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <nlohmann/json.hpp>

namespace engine::animations {
    class FrameAnimation {
    public:
        FrameAnimation();
        FrameAnimation(const nlohmann::json &description);
        FrameAnimation(const FrameAnimation &frameAnimation) {
            for (auto frame : frameAnimation.m_frames) {
                frame.scale.applied = false;
                frame.rotation.applied = false;
                m_frames.push_back(frame);
            }
        }

        void addFrame(const nlohmann::json &description);

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

    private:
        struct Frame {
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

            template<typename T>
            struct RelativeOrAbsolute {
                T data;
                bool relative;
                bool applied = false; // We only want to apply the relative modification once
            };

            sf::IntRect textureRect;
            bool applyTextureRect = false;
            sf::Vector2f origin;
            bool applyOrigin = false;
            RelativeOrAbsolute<sf::Vector2f> scale;
            bool applyScale = false;
            RelativeOrAbsolute<float> rotation;
            bool applyRotation = false;
            sf::Color color;
            bool applyColor = false;
            float relativeDuration;
        };

    private:
        std::list<Frame> m_frames;
    };
}