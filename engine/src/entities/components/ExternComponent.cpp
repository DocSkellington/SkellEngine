#include "entities/components/ExternComponent.h"

namespace engine::entities::components {
    ExternComponent::ExternComponent() : Component() {

    }

    ExternComponent::~ExternComponent() {

    }

    void ExternComponent::create(Context &context, const nlohmann::json &jsonTable) {
        this->jsonTable = jsonTable;
    }

    void ExternComponent::create(Context &context, const sol::table& luaTable) {
        this->luaTable = luaTable;
    }

    void ExternComponent::create(Context &context, const nlohmann::json &jsonTable, const sol::table& luaTable) {
        this->jsonTable = jsonTable;
        this->luaTable = luaTable;
    }
}