#pragma once

#include "entities/components/Component.h"
#include "Context.h"

namespace engine::entities::components {
    /**
    * \brief Describes the position of a component in the world
    */
    class PositionComponent : public Component {
    public:
        explicit PositionComponent();
        PositionComponent(const Component&) = delete;
        virtual ~PositionComponent();

        virtual void create(Context &context, const nlohmann::json &jsonTable) override;
        virtual void create(Context &context, const sol::table& luaTable) override;
        virtual void create(Context &context, const nlohmann::json &jsonTable, const sol::table& luaTable) override;

    private:
        float m_x, m_y;
        static Component::RegisterComponent<PositionComponent> rcpc;
    };
}