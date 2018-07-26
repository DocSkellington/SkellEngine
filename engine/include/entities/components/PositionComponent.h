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

        /*
        virtual void set(const std::string &name, long value) override;
        virtual void set(const std::string &name, double value) override;
        virtual void set(const std::string &name, bool value) override;
        virtual void set(const std::string &name, const std::string &value) override;

        virtual void set(const std::string &name, sol::nil_t value) override;
        virtual void set(const std::string &name, const sol::table &value) override;

        virtual void set(const std::string &name, nlohmann::json value) override;
        */

        virtual std::pair<long, bool> getInt(const std::string &name) override;
        virtual std::pair<double, bool> getFloat(const std::string &name) override;
        virtual std::pair<bool, bool> getBool(const std::string &name) override;
        virtual std::pair<std::string, bool> getString(const std::string &name) override;
        virtual std::pair<sol::object, bool> getObject(const std::string &name) override;

    private:
        double m_x, m_y;
        static Component::RegisterComponent<PositionComponent> rcpc;
    };
}