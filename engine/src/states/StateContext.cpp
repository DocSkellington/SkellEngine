#include "SkellEngine/states/StateContext.h"

#include "SkellEngine/levels/Level.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/entities/EntityManager.h"

namespace engine::states {
    StateContext::StateContext(Context &context, const std::string &stateName) :
        context(context),
        stateName(stateName) {
        entityManager = std::make_shared<entities::EntityManager>(*this);
        tmx::Logger::log("Entity manager ready");
        systemManager = std::make_shared<systems::SystemManager>(*this);
        tmx::Logger::log("System manager ready");
        level = std::make_shared<levels::Level>(*this);
        tmx::Logger::log("Level system ready");
            
        if (context.window) {
            gui = std::make_shared<tgui::Gui>(*context.window);
            tmx::Logger::log("Graphical user interface system ready");
        }
    }
}