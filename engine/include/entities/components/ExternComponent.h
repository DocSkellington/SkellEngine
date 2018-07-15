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

        void create(Context &context, const nlohmann::json &jsonTable) override;

        void set(const std::string &name, long value);
        void set(const std::string &name, unsigned long value);
        void set(const std::string &name, std::string value);
        void set(const std::string &name, bool value);

        void set(const std::string &name, sol::nil_t value);
        void set(const std::string &name, sol::table value);

        void set(const std::string &name, nlohmann::json value);

        int get(const std::string &name);

        static void luaFunctions(sol::state &lua);

    private:
        nlohmann::json m_jsonTable;
    };
}