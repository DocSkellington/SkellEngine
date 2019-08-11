#include "SkellEngine/states/StateContext.h"

#include "SkellEngine/levels/Level.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/entities/EntityManager.h"

namespace engine::states {
    StateContext::StateContext(Context &context) :
        context(context) {
        entityManager = std::make_shared<entities::EntityManager>(*this);
        tmx::Logger::log("Entity manager ready");
        systemManager = std::make_shared<systems::SystemManager>(*this);
        tmx::Logger::log("System manager ready");
        level = std::make_shared<levels::Level>(*this);
        tmx::Logger::log("Level system ready");
    }
}