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
        struct StateContext {
            StateContext(engine::Context &context);

            engine::Context &context;
            std::shared_ptr<entities::EntityManager> entityManager;
            std::shared_ptr<systems::SystemManager> systemManager;
            std::shared_ptr<levels::Level> level;
        };
    }
}