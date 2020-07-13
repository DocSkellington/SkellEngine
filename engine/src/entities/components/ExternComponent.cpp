#include "SkellEngine/entities/components/ExternComponent.hpp"
#include <sol/sol.hpp>

#include "SkellEngine/utilities/json_lua.hpp"
#include "SkellEngine/Context.hpp"

namespace engine::entities::components {
    ExternComponent::ExternComponent(states::StateContext &context) :
        Component(context) {

    }

    ExternComponent::~ExternComponent() {

    }

    void ExternComponent::create(const nlohmann::json &jsonTable) {
        loadFromJSON(jsonTable);
    }

    void ExternComponent::luaFunctions(sol::state &lua) {
        lua.new_usertype<ExternComponent>("ExternComponent",
            sol::base_classes, sol::bases<utilities::VariableStorage, components::Component>()
        );
    }

    std::string ExternComponent::getLogErrorPrefix() const {
        return "ExternComponent";
    }
}