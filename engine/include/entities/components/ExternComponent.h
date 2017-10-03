#pragma once

#include "entities/components/Component.h"

namespace engine::entities::components {
    /**
     * \brief This component is defined only through JSON and Lua scripts.
     * 
     * Its purpose is to be used in systems created by the user.
     */
    class ExternComponent : public Component {
    public:
        ExternComponent();
        ExternComponent(const ExternComponent&) = delete;
        virtual ~ExternComponent();

        void create(const nlohmann::json &jsonTable);
        void create(const sol::table& luaTable);
        void create(const nlohmann::json &jsonTable, const sol::table& luaTable);

    private:
        nlohmann::json jsonTable;
        sol::table luaTable;
    };
}