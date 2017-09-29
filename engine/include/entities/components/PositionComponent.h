#pragma once

#include "entities/components/Component.h"

namespace engine::entities::components {
    /**
    * \brief The base of every component
    */
    class PositionComponent : public Component {
    public:
        explicit PositionComponent();
        PositionComponent(const Component&) = delete;
        virtual ~PositionComponent();

        virtual void create(const nlohmann::json &jsonTable);
        virtual void create(const sol::table& luaTable);
        virtual void create(const nlohmann::json &jsonTable, const sol::table& luaTable);

    private:
        float m_x, m_y;
        static Component::RegisterComponent<PositionComponent> rcpc;
    };
}