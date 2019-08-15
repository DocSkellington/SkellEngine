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
        // TODO: handle case there is no entity
        auto entity = event.getEntity(0);
        if (auto anim = entity->getComponent("animation") ; anim) {
            auto animation = std::static_pointer_cast<entities::components::AnimationComponent>(anim);
            animation->getAnimator().play(event.getString("animation").first);
        }
    }

    void AnimationSystem::stop(const events::Event &event) {
        auto entity = event.getEntity(0);
        if (auto anim = entity->getComponent("animation") ; anim) {
            auto animation = std::static_pointer_cast<entities::components::AnimationComponent>(anim);
            animation->getAnimator().stop();
        }
    }

    void AnimationSystem::pause(const events::Event &event) {
        auto entity = event.getEntity(0);
        if (auto anim = entity->getComponent("animation") ; anim) {
            auto animation = std::static_pointer_cast<entities::components::AnimationComponent>(anim);
            animation->getAnimator().pause();
        }
    }

    void AnimationSystem::resume(const events::Event &event) {
        auto entity = event.getEntity(0);
        if (auto anim = entity->getComponent("animation") ; anim) {
            auto animation = std::static_pointer_cast<entities::components::AnimationComponent>(anim);
            animation->getAnimator().resume();
        }
    }
}