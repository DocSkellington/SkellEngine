#include "SkellEngine/entities/components/ExternComponent.hpp"
#include <sol/sol.hpp>

#include "SkellEngine/utilities/json_lua.hpp"
#include "SkellEngine/Context.hpp"

namespace engine::entities::components {
    ExternComponent::ExternComponent(states::StateContext &context) :
        MemberStorage(context.context),
        Component(context),
        ExternMemberStorage(context.context) {

    }

    ExternComponent::~ExternComponent() {

    }

    void ExternComponent::create(const nlohmann::json &jsonTable) {
        setInternalJSONTable(jsonTable);
    }

    void ExternComponent::luaFunctions(sol::state &lua) {
        lua.new_usertype<ExternComponent>("ExternComponent",
            sol::base_classes, sol::bases<utilities::ExternMemberStorage, components::Component>()
        );
    }

    std::string ExternComponent::getLogErrorPrefix() const {
        return "ExternComponent";
    }
}