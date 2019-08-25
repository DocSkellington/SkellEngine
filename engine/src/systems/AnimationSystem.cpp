#include "SkellEngine/systems/AnimationSystem.h"

#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/entities/components/AnimationComponent.h"
#include "SkellEngine/entities/components/GraphicalSpriteSheetComponent.h"

namespace engine::systems {
    AnimationSystem::AnimationSystem(SystemManager &manager) :
        System(manager) {
        registerCallback("PlayAnimation", [this](const events::Event &event) { this->play(event); }, manager.getContext().stateName);
        registerCallback("StopAnimation", [this](const events::Event &event) { this->stop(event); }, manager.getContext().stateName);
        registerCallback("PauseAnimation", [this](const events::Event &event) { this->pause(event); }, manager.getContext().stateName);
        registerCallback("ResumeAnimation", [this](const events::Event &event) { this->resume(event); }, manager.getContext().stateName);
    }
    
    bool AnimationSystem::update(sf::Int64 deltaTime, sf::View&) {
        for (auto entity : getEntities()) {
            if (auto anim = entity->getComponent("animation") ; anim) {
                auto animation = std::static_pointer_cast<entities::components::AnimationComponent>(anim);
                animation->getAnimator().update(deltaTime);

                if (auto sprite = entity->getComponent("spritesheet") ; sprite) {
                    auto spritesheet = std::static_pointer_cast<entities::components::GraphicalSpriteSheetComponent>(sprite);
                    animation->getAnimator().animate(spritesheet->getSprite());
                }
            }
        }
        return true;
    }

    bool AnimationSystem::checkComponents(engine::entities::Entity::Ptr entity) const {
        return entity->hasComponent("spritesheet") && entity->hasComponent("animation");
    }

    void AnimationSystem::play(const events::Event &event) {
        if (event.getNumberOfEntities() != 1) {
            throw std::invalid_argument("AnimationSystem: invalid number of entities joined in the event of type " + event.getType() + ": there should be exactly 1 entity");
        }
        auto entity = event.getEntity(0);

        if (!entity) {
            throw std::invalid_argument("AnimationSystem: play: the given entity is a null pointer");
        }

        if (auto anim = entity->getComponent("animation") ; anim) {
            auto animation = std::static_pointer_cast<entities::components::AnimationComponent>(anim);
            const auto &map = animation->getAnimationMap();
            auto &animator = animation->getAnimator();

            if (!event.has("animation")) {
                throw std::invalid_argument("AnimationSystem: play: invalid event: the event must have a field 'animation'");
            }
            
            animations::Animator<sf::Sprite>::QueueKey key = 0;
            try {
                if (auto k = event.getInt("queue") ; k.second) {
                    key = k.first;
                }
            }
            catch (const errors::WrongType &e) {
                throw std::invalid_argument("AnimationSystem: play: the 'queue' animation must be an integer.");
            }

            try {
                auto animationDescription = event.getString("animation");
                animator.play(key, map.getAnimation(animationDescription.first));
            }
            catch (const errors::WrongType &e) {
                throw std::invalid_argument("AnimationSystem: play: the 'animation' field must be a string.");
            }
        }
        else {
            throw std::invalid_argument("AnimationSystem: play: invalid event: the joined event must have an animation component");
        }
    }

    void AnimationSystem::stop(const events::Event &event) {
        auto entity = event.getEntity(0);
        if (auto anim = entity->getComponent("animation") ; anim) {
            auto animation = std::static_pointer_cast<entities::components::AnimationComponent>(anim);

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
                throw std::invalid_argument("AnimationSystem: stop: the 'queue' field must be an integer");
            }
        }
    }

    void AnimationSystem::pause(const events::Event &event) {
        auto entity = event.getEntity(0);
        if (auto anim = entity->getComponent("animation") ; anim) {
            auto animation = std::static_pointer_cast<entities::components::AnimationComponent>(anim);

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
                throw std::invalid_argument("AnimationSystem: pause: the 'queue' field must be an integer");
            }
        }
    }

    void AnimationSystem::resume(const events::Event &event) {
        auto entity = event.getEntity(0);
        if (auto anim = entity->getComponent("animation") ; anim) {
            auto animation = std::static_pointer_cast<entities::components::AnimationComponent>(anim);

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
                throw std::invalid_argument("AnimationSystem: resume: the 'queue' field must be an integer");
            }
        }
    }
}