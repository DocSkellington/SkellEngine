#include "SkellEngine/states/StateContext.hpp"

#include "SkellEngine/levels/Level.hpp"
#include "SkellEngine/systems/SystemManager.hpp"
#include "SkellEngine/entities/EntityManager.hpp"

namespace engine::states {
    StateContext::StateContext(Context &context, const std::string &stateName) :
        context(context),
        stateName(stateName) {
        entityManager = std::make_shared<entities::EntityManager>(*this);
        context.logger.log("Entity manager ready");
        systemManager = std::make_shared<systems::SystemManager>(*this);
        context.logger.log("System manager ready");
        level = std::make_shared<levels::Level>(*this);
        context.logger.log("Level system ready");
            
        if (context.window) {
            gui = std::make_shared<tgui::Gui>(*context.window);
            context.logger.log("Graphical user interface system ready");
        }
    }
}