#include "entities/components/Component.h"

#include "entities/components/ExternComponent.h"

namespace engine::entities::components {
    Component::Component() {

    }

    Component::~Component() {

    }

    void Component::set(const std::string &name, std::any value) {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            std::cout << itr->second.first.name() << '\n';
            // We check that we can construct the data
            if (std::is_constructible_v<itr->second.first, value.type()>)
        }
    }

    void Component::setContext(Context &context) {
        m_context = &context;
    }

    Component::Ptr Component::createInstance(Context &context, const std::string &componentType) {
        auto compoConstructor = getMapToComponent()->find(componentType);
        Component::Ptr ptr;
        if (compoConstructor == getMapToComponent()->end()) {
            ptr = std::make_shared<ExternComponent>();
        }
        else {
            ptr = compoConstructor->second();
        }
        ptr->setContext(context);
        return ptr;
    }

    void Component::luaFunctions(sol::state &lua) {
        lua.new_usertype<Component>("component",
            "get", &ExternComponent::getObject

            /*
            "set", sol::overload(
                sol::resolve<void(const std::string&, long)>(&Component::set),
                sol::resolve<void(const std::string&, double)>(&Component::set),
                sol::resolve<void(const std::string&, bool)>(&Component::set),
                sol::resolve<void(const std::string&, const std::string&)>(&Component::set),
                sol::resolve<void(const std::string&, const sol::table&)>(&Component::set),
                sol::resolve<void(const std::string&, sol::nil_t)>(&Component::set)
            )
            */
        );
    }

    Context &Component::getContext() {
        return *m_context;
    }

    std::shared_ptr<Component::MapType> Component::getMapToComponent() {
        static std::shared_ptr<Component::MapType> map = std::make_shared<Component::MapType>();
        return map;
    }
}