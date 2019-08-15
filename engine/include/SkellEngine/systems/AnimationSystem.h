#pragma once

#include "SkellEngine/systems/System.h"
#include "SkellEngine/animations/AnimationMap.h"
#include "SkellEngine/events/Event.h"

namespace engine::systems {
    /**
     * \brief Animates a sprite
     */
    class AnimationSystem : public System {
    public:
        AnimationSystem(SystemManager &manager);

        virtual bool update(sf::Int64 deltaTime, sf::View &view) override;

    protected:
        virtual bool checkComponents(engine::entities::Entity::Ptr entity) const override;

        virtual void play(const events::Event &event);
        virtual void stop(const events::Event &event);
        virtual void pause(const events::Event &event);

    private:
        REGISTER_SYSTEM(AnimationSystem, "animation")
    };
}