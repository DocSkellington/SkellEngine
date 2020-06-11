#pragma once

#include "SkellEngine/systems/System.hpp"
#include "SkellEngine/animations/AnimationMap.hpp"
#include "SkellEngine/events/Event.hpp"
#include "SkellEngine/entities/components/AnimationComponent.hpp"

namespace engine::systems {
    /**
     * \brief A system that animates entities
     * 
     * The entities must have a entities::components::AnimationComponent and a entities::components::GraphicalSpriteSheetComponent
     */
    class AnimationSystem : public System {
    public:
        /**
         * \brief Constructor
         * \param manager The system manager
         */
        AnimationSystem(SystemManager &manager);

        virtual bool update(sf::Int64 deltaTime, sf::View &view) override;

    protected:
        virtual bool checkComponents(engine::entities::Entity::Ptr entity) const override;

        /**
         * \brief Asks the animator of the entity sent with the event to play an animation
         * \param event The event
         * \throws errors::InvalidEvent if the event does not have exactly one entity, if the entity does not contain an animation component or if the animation description is incorrect
         */
        virtual void play(const events::Event &event);
        /**
         * \brief Asks the animator of the entity sent with the event to queue an animation
         * \param event The event
         * \throws errors::InvalidEvent if the event does not have exactly one entity, if the entity does not contain an animation component or if the animation description is incorrect
         */
        virtual void queue(const events::Event &event);
        /**
         * \brief Asks the animator of the entity sent with the event to stop (a queue)
         * \param event The event
         * \throws errors::InvalidEvent if the event does not have exactly one entity, if the entity does not contain an animation component or if the queue key is incorrect (if present)
         */
        virtual void stop(const events::Event &event);
        /**
         * \brief Asks the animator of the entity sent with the event to pause (a queue)
         * \param event The event
         * \throws errors::InvalidEvent if the event does not have exactly one entity, if the entity does not contain an animation component or if the queue key is incorrect (if present)
         */
        virtual void pause(const events::Event &event);
        /**
         * \brief Asks the animator of the entity sent with the event to resume (a queue)
         * \param event The event
         * \throws errors::InvalidEvent if the event does not have exactly one entity, if the entity does not contain an animation component or if the queue key is incorrect (if present)
         */
        virtual void resume(const events::Event &event);
        /**
         * \brief Asks the animator of the entity sent with the event to enable the default animation
         * \param event The event
         * \throws errors::InvalidEvent if the event does not have exactly one entity or if the entity does not contain an animation component
         */
        virtual void enableDefault(const events::Event &event);
        /**
         * \brief Asks the animator of the entity sent with the event to disable the default animation
         * \param event The event
         * \throws errors::InvalidEvent if the event does not have exactly one entity or if the entity does not contain an animation component
         */
        virtual void disableDefault(const events::Event &event);
        /**
         * \brief Asks the animator of the entity sent with the event to set the default animation according to the event
         * \param event The event
         * \throws errors::InvalidEvent if the event does not have exactly one entity, if the entity does not contain an animation component or if the animation description is incorrect
         */
        virtual void setDefault(const events::Event &event);

        /**
         * \brief Extracts the animation component from the entity sent with an event
         * \param event The event
         * \return The animation component
         * \throws errors::InvalidEvent if the event does not have exactly one entity or if the entity does not contain an animation component
         */
        std::shared_ptr<entities::components::AnimationComponent> getAnimationComponent(const events::Event &event) const;

    private:
        REGISTER_SYSTEM(AnimationSystem, "animation")
    };
}