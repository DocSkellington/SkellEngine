#include "SkellEngine/systems/AnimationSystem.h"

#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/entities/components/GraphicalSpriteSheetComponent.h"
#include "SkellEngine/errors/InvalidEvent.h"

namespace engine::systems {
    AnimationSystem::AnimationSystem(SystemManager &manager) :
        System(manager) {
        registerCallback("PlayAnimation", std::bind(&AnimationSystem::play, this, std::placeholders::_1), manager.getContext().stateName);
        registerCallback("StopAnimation", std::bind(&AnimationSystem::stop, this, std::placeholders::_1), manager.getContext().stateName);
        registerCallback("PauseAnimation", std::bind(&AnimationSystem::pause, this, std::placeholders::_1), manager.getContext().stateName);
        registerCallback("ResumeAnimation", std::bind(&AnimationSystem::resume, this, std::placeholders::_1), manager.getContext().stateName);
        registerCallback("EnableDefaultAnimation", std::bind(&AnimationSystem::enableDefault, this, std::placeholders::_1), manager.getContext().stateName);
        registerCallback("DisableDefaultAnimation", std::bind(&AnimationSystem::disableDefault, this, std::placeholders::_1), manager.getContext().stateName);
        registerCallback("SetDefaultAnimation", std::bind(&AnimationSystem::setDefault, this, std::placeholders::_1), manager.getContext().stateName);
    }
    
    bool AnimationSystem::update(sf::Int64 deltaTime, sf::View&) {
        for (auto entity : getEntities()) {
            if (auto anim = entity->getComponent("animation") ; anim) {
                auto animation = std::static_pointer_cast<entities::components::AnimationComponent>(anim);
                auto eventsToSend = animation->getAnimator().update(deltaTime);

                if (auto sprite = entity->getComponent("spritesheet") ; sprite) {
                    auto spritesheet = std::static_pointer_cast<entities::components::GraphicalSpriteSheetComponent>(sprite);
                    animation->getAnimator().animate(spritesheet->getSprite());
                }

                for (auto &event : eventsToSend) {
                    getSystemManager().getContext().context.eventHandler->sendEvent(event, {entity});
                }
            }
        }
        return true;
    }

    bool AnimationSystem::checkComponents(engine::entities::Entity::Ptr entity) const {
        return entity->hasComponent("spritesheet") && entity->hasComponent("animation");
    }

    void AnimationSystem::play(const events::Event &event) {
        auto animation = getAnimationComponent(event);
        const auto &map = animation->getAnimationMap();
        auto &animator = animation->getAnimator();

        if (!event.has("animation")) {
            throw errors::InvalidEvent("AnimationSystem: play: invalid event: the event must have a field 'animation'");
        }
        
        animations::Animator<sf::Sprite>::QueueKey key = 0;
        try {
            if (auto k = event.getInt("queue") ; k.second) {
                key = k.first;
            }
        }
        catch (const errors::WrongType &e) {
            throw errors::InvalidEvent("AnimationSystem: play: the 'queue' animation must be an integer.");
        }

        try {
            auto animationDescription = event.getString("animation");
            animator.play(key, map.getAnimation(animationDescription.first));
        }
        catch (const errors::WrongType &e) {
            throw errors::InvalidEvent("AnimationSystem: play: the 'animation' field must be a string.");
        }
    }

    void AnimationSystem::queue(const events::Event &event) {
        auto animation = getAnimationComponent(event);
        const auto &map = animation->getAnimationMap();
        auto &animator = animation->getAnimator();

        if (!event.has("animation")) {
            throw errors::InvalidEvent("AnimationSystem: play: invalid event: the event must have a field 'animation'");
        }
        
        animations::Animator<sf::Sprite>::QueueKey key = 0;
        try {
            if (auto k = event.getInt("queue") ; k.second) {
                key = k.first;
            }
        }
        catch (const errors::WrongType &e) {
            throw errors::InvalidEvent("AnimationSystem: play: the 'queue' animation must be an integer.");
        }

        try {
            auto animationDescription = event.getString("animation");
            animator.queue(key, map.getAnimation(animationDescription.first));
        }
        catch (const errors::WrongType &e) {
            throw errors::InvalidEvent("AnimationSystem: play: the 'animation' field must be a string.");
        }
    }

    void AnimationSystem::stop(const events::Event &event) {
        auto animation = getAnimationComponent(event);

        try {
            if (auto k = event.getInt("queue") ; k.second) {
                // Queue defined. We stop the queue
                animation->getAnimator().stop(k.first);
            }
            else {
                // No queue defined. We stop the whole animator
                animation->getAnimator().stop();
            }
        }
        catch (const errors::WrongType &e) {
            throw errors::InvalidEvent("AnimationSystem: stop: the 'queue' field must be an integer");
        }
    }

    void AnimationSystem::pause(const events::Event &event) {
        auto animation = getAnimationComponent(event);

        try {
            if (auto k = event.getInt("queue") ; k.second) {
                // Queue defined. We pause the queue
                animation->getAnimator().pause(k.first);
            }
            else {
                // No queue defined. We pause the whole animator
                animation->getAnimator().pause();
            }
        }
        catch (const errors::WrongType &e) {
            throw errors::InvalidEvent("AnimationSystem: pause: the 'queue' field must be an integer");
        }
    }

    void AnimationSystem::resume(const events::Event &event) {
        auto animation = getAnimationComponent(event);

        try {
            if (auto k = event.getInt("queue") ; k.second) {
                // Queue defined. We resume the queue
                animation->getAnimator().resume(k.first);
            }
            else {
                // No queue defined. We resume the whole animator
                animation->getAnimator().resume();
            }
        }
        catch (const errors::WrongType &e) {
            throw errors::InvalidEvent("AnimationSystem: resume: the 'queue' field must be an integer");
        }
    }

    void AnimationSystem::enableDefault(const events::Event &event) {
        auto animation = getAnimationComponent(event);
        animation->getAnimator().enableDefault();
    }

    void AnimationSystem::disableDefault(const events::Event &event) {
        auto animation = getAnimationComponent(event);
        animation->getAnimator().disableDefault();
    }

    void AnimationSystem::setDefault(const events::Event &event) {
        auto animation = getAnimationComponent(event);
            if (event.has("animation")) {
                try {
                    const std::string &defaultAnimation = event.getString("animation").first;
                    if (animation->getAnimationMap().hasAnimation(defaultAnimation)) {
                        animation->getAnimator().setDefault(animation->getAnimationMap().getAnimation(defaultAnimation));
                    }
                    else {
                        throw errors::InvalidEvent("AnimationSystem: setDefaultAnimation: the animation " + defaultAnimation + " is unknown for the received entity");
                    }
                }
                catch (const errors::WrongType &e) {
                    throw errors::InvalidEvent("AnimationSystem: setDefaultAnimation: the 'animation' field must be a string");
                }
            }
            else {
                throw errors::InvalidEvent("AnimationSystem: setDefaultAnimation: the event must have a field 'animation'");
            }
    }

    std::shared_ptr<entities::components::AnimationComponent> AnimationSystem::getAnimationComponent(const events::Event &event) const {
        if (event.getNumberOfEntities() != 1) {
            throw errors::InvalidEvent("AnimationSystem: invalid number of entities joined in the event of type " + event.getType() + ": there should be exactly 1 entity");
        }
        auto entity = event.getEntity(0);

        if (!entity) {
            throw errors::InvalidEvent("AnimationSystem: invalid event: the given entity is a null pointer");
        }

        if (auto anim = entity->getComponent("animation") ; anim) {
            return std::static_pointer_cast<entities::components::AnimationComponent>(anim);
        }
        else {
            throw errors::InvalidEvent("AnimationSystem: invalid event: the joined event must have an animation component");
        }
    }
}