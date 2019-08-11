#pragma once

#include <memory>

#include "SkellEngine/Context.h"

namespace engine {
    namespace systems {
        class SystemManager;
    }
    namespace entities {
        class EntityManager;
    }
    namespace levels {
        class Level;
    }

    namespace states {
        /**
         * \brief The context of a state
         */
        struct StateContext {
            /**
             * \brief Constructor
             * \param context The global context
             */
            StateContext(engine::Context &context);

            /**
             * \brief The global context
             */
            engine::Context &context;
            /**
             * \brief The entity manager
             */
            std::shared_ptr<entities::EntityManager> entityManager;
            /**
             * \brief The system manager
             */
            std::shared_ptr<systems::SystemManager> systemManager;
            /**
             * \brief The level
             */
            std::shared_ptr<levels::Level> level;
        };
    }
}