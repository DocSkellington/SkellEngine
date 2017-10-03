#include "entities/components/ExternComponent.h"

namespace engine::entities::components {
    ExternComponent::ExternComponent() {

    }

    ExternComponent::~ExternComponent() {

    }

    void ExternComponent::create(const nlohmann::json &jsonTable) {
        this->jsonTable = jsonTable;
    }

    void ExternComponent::create(const sol::table& luaTable) {
        this->luaTable = luaTable;
    }

    void ExternComponent::create(const nlohmann::json &jsonTable, const sol::table& luaTable) {
        this->jsonTable = jsonTable;
        this->luaTable = luaTable;
    }
}