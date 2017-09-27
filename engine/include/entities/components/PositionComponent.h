#pragma once

#include "entities/components/Component.h"

namespace engine::entities::components {
    /**
    * \brief The base of every component
    */
    class PositionComponent : public Component {
    public:
        explicit PositionComponent(const sol::table &luaTable);
        explicit PositionComponent(const nlohmann::json &jsonTable);
        explicit PositionComponent(const sol::table &luaTable, const nlohmann::json &jsonTable);
        PositionComponent(const Component&) = delete;
        virtual ~PositionComponent();
    };
}