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

        virtual void set(const std::string &name, int value) override;
        virtual void set(const std::string &name, const std::string &value) override;
        virtual void set(const std::string &name, bool value) override;

        virtual void set(const std::string &name, sol::nil_t value) override;
        virtual void set(const std::string &name, const sol::table &value) override;

        virtual void set(const std::string &name, nlohmann::json value) override;

        virtual std::tuple<int, bool> getInt(const std::string &name) override;
        virtual std::tuple<sol::object, bool> getObject(const std::string &name) override;

    private:
        float m_x, m_y;
        static Component::RegisterComponent<PositionComponent> rcpc;
    };
}