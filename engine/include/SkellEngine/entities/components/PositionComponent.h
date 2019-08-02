#pragma once

#include "SkellEngine/entities/components/Component.h"
#include "SkellEngine/Context.h"

namespace engine::entities::components {
    /**
    * \brief Describes the position of a component in the world
    */
    class PositionComponent : public Component {
    public:
        explicit PositionComponent(Context &);
        PositionComponent(const Component&) = delete;
        virtual ~PositionComponent();

        virtual void create(const nlohmann::json &jsonTable) override;

    private:
        double m_x, m_y;

        REGISTER_COMPONENT(PositionComponent, "position")
    };
}