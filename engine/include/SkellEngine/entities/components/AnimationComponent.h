#pragma once

#include "SkellEngine/entities/components/Component.h"
#include "SkellEngine/animations/AnimationMap.h"
#include "SkellEngine/animations/Animator.h"

namespace engine::entities::components {
    /*
     * Only supports three animation types!
     * 
     *  "default": "idle" // If not present, animation stuck on the last frame
     * 
     *  + allow overwriting
     */
    class AnimationComponent : public Component {
    public:
        using Animator = animations::Animator<sf::Sprite>;
        using AnimationMap = animations::AnimationMap<sf::Sprite, std::string>;

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