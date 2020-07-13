#include "SkellEngine/entities/components/Component.hpp"

#include "SkellEngine/entities/components/ExternComponent.hpp"
#include "SkellEngine/Context.hpp"

namespace engine::entities::components {
    Component::Component(states::StateContext &context) :
        VariableStorage(context.context)
        {

    }

    Component::~Component() {

    }

    Component::Ptr Component::createInstance(states::StateContext &context, const std::string &componentType) {
        Ptr ptr = RegisteredComponents::construct(componentType, context);

        if (!ptr) {
            context.context.logger.log("Component: createInstance: creating an ExternComponent since " + componentType + " is not registered");
            ptr = std::make_shared<ExternComponent>(context);
        }
        return ptr;
    }

    std::string Component::getLogErrorPrefix() const {
        return "Component";
    }

    void Component::luaFunctions(sol::state &lua) {
        lua.new_usertype<Component>("component",
            sol::base_classes, sol::bases<utilities::VariableStorage>()
        );
    }
}