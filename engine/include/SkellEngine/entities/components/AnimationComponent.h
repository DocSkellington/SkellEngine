#pragma once

#include "SkellEngine/entities/components/Component.h"
#include "SkellEngine/animations/AnimationMap.h"
#include "SkellEngine/animations/Animator.h"

namespace engine::entities::components {
    /*
     * 
     *  "default": "idle" // If not present, animation stuck on the last frame
     *  "walk": {
     *      "type": "frame"
     *      "duration": time,
     * 
     *      "loop": true
     *      OR
     *      "repeat": number
     * 
     *      "frames": [
     *          // first frame
     *          // If just rect, it's easy: use thor::FrameAnimation
     *          // Otherwise, create a specific animation
     *          {
     *              "smooth": true
     *              "repeated": true
     * 
     *              "origin": [x, y]
     * 
     *              "color": "Color" OR [255, 255, 255] OR [255, 255, 255, 255]
     * 
     *              "scale": [x, y]
     *              
     *              "rotation": absolute OR "+relative" OR "-relative"
     * 
     *              "duration": time
     *          },
     *          // next frame
     *          ...
     *      ]
     *  }
     * 
     *  + allow overwriting
     */
    class AnimationComponent : public Component {
    public:
        using Animator = animations::Animator<sf::Sprite, std::string>;
        using AnimationMap = Animator::AnimationMap;

    public:
        AnimationComponent(states::StateContext &stateContext);

        virtual void create(const nlohmann::json &description) override;

        AnimationMap& getAnimationMap();
        const AnimationMap& getAnimationMap() const;

        Animator& getAnimator();
        const Animator &getAnimator() const;

    private:
        AnimationMap m_animations;
        Animator m_animator;

        REGISTER_COMPONENT(AnimationComponent, "animation")
    };
}