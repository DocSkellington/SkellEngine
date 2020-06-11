#pragma once

#include <memory>

#include "SkellEngine/Context.hpp"

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
             * \param stateName The name of the state using this context
             */
            StateContext(engine::Context &context, const std::string &stateName);

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

            /**
             * \brief The Graphical User Interface tool
             */
            std::shared_ptr<tgui::Gui> gui;

            /**
             * \brief The name of the state using this StateContext
             */
            std::string stateName;
        };
    }
}