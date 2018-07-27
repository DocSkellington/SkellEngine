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

        virtual void create(const nlohmann::json &jsonTable) override;

    private:
        double m_x, m_y;
        int m_int;
        static Component::RegisterComponent<PositionComponent> rcpc;
    };
}