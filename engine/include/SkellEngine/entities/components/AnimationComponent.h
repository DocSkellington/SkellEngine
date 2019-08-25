#pragma once

#include "SkellEngine/entities/components/Component.h"
#include "SkellEngine/animations/AnimationMap.h"
#include "SkellEngine/animations/Animator.h"

namespace engine::entities::components {
    /**
     * \brief A component describing the animation of an entity
     * 
     * The entity must have a spritesheet component.
     * 
     * This component supports three animation types:
     *  - ColorAnimation
     *  - FadeAnimation
     *  - FrameAnimation
     */
    class AnimationComponent : public Component {
    public:
        /**
         * \brief The type of the animator of this component
         */
        using Animator = animations::Animator<sf::Sprite>;
        /**
         * \brief The type of structure that stores the animations
         */
        using AnimationMap = animations::AnimationMap<sf::Sprite, std::string>;

    public:
        /**
         * \brief Constructor
         * \param stateContext The context of the state
         */
        AnimationComponent(states::StateContext &stateContext);

        virtual void create(const nlohmann::json &description) override;

        /** @{ */
        /**
         * \brief Gets the structure that stores the animations
         * \return The animation map
         */
        AnimationMap& getAnimationMap();
        const AnimationMap& getAnimationMap() const;
        /** @} */

        /** @{ */
        /**
         * \brief Gets the animator
         * \return The animator
         */
        Animator& getAnimator();
        const Animator &getAnimator() const;
        /** @} */

    private:
        AnimationMap m_animations;
        Animator m_animator;

        REGISTER_COMPONENT(AnimationComponent, "animation")
    };
}