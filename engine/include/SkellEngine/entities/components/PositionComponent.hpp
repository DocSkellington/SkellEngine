#pragma once

#include "SkellEngine/entities/components/Component.hpp"
#include "SkellEngine/Context.hpp"

namespace engine::entities::components {
    /**
    * \brief Describes the position of a component in the world
    */
    class PositionComponent : public Component {
    public:
        /**
         * \brief The constructor
         * \param context The context of the engine
         */
        explicit PositionComponent(states::StateContext &context);
        PositionComponent(const Component&) = delete;
        virtual ~PositionComponent();

        virtual void create(const nlohmann::json &jsonTable) override;

    private:
        double m_x, m_y;
        bool m_absolute;

        REGISTER_COMPONENT(PositionComponent, "position")
    };
}