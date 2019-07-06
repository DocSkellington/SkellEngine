#include "SkellEngine/entities/components/Component.h"

#include "SkellEngine/entities/components/ExternComponent.h"
#include "SkellEngine/Context.h"

namespace engine::entities::components {
    Component::Component(Context &context) :
        MemberStorage(context)
        {

    }

    Component::~Component() {

    }

    Component::Ptr Component::createInstance(Context &context, const std::string &componentType) {
        auto compoConstructor = getMapToComponent()->find(componentType);
        Component::Ptr ptr;
        if (compoConstructor == getMapToComponent()->end()) {
            ptr = std::make_shared<ExternComponent>(context);
        }
        else {
            ptr = compoConstructor->second(context);
        }
        return ptr;
    }

    std::shared_ptr<Component::MapType> Component::getMapToComponent() {
        static std::shared_ptr<Component::MapType> map = std::make_shared<Component::MapType>();
        return map;
    }

    std::string Component::getLogErrorPrefix() const {
        return "Component";
    }

    void Component::luaFunctions(sol::state &lua) {
        lua.new_usertype<Component>("component",
            sol::base_classes, sol::bases<utilities::MemberStorage>()
        );
    }
}