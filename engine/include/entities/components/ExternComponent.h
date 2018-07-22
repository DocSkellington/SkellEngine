#pragma once

#include "entities/components/Component.h"
#include "Context.h"

namespace engine::entities::components {
    /**
     * \brief This component is defined only through JSON.
     * 
     * It can be modified in Lua and C++
     * Its purpose is to be used in systems created by the user.
     */
    class ExternComponent : public Component {
    public:
        ExternComponent();
        ExternComponent(const ExternComponent&) = delete;
        virtual ~ExternComponent();

        void create(const nlohmann::json &jsonTable) override;

        void set(const std::string &name, int value) override;
        void set(const std::string &name, const std::string& value) override;
        void set(const std::string &name, bool value) override;

        void set(const std::string &name, sol::nil_t value) override;
        void set(const std::string &name, const sol::table& value) override;

        void set(const std::string &name, nlohmann::json value) override;

        std::tuple<int, bool> getInt(const std::string &name) override;
        std::tuple<sol::object, bool> getObject(const std::string &name) override;

    private:
        nlohmann::json m_jsonTable;
    };
}