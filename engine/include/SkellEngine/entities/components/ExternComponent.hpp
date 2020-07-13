#pragma once

#include "SkellEngine/entities/components/Component.hpp"
#include "SkellEngine/Context.hpp"

namespace engine::entities::components {
    /**
     * \brief This component is defined only through JSON.
     * 
     * It can be modified in Lua and C++
     * Its purpose is to be used in systems created by the user.
     */
    class ExternComponent : public Component {
    public:
        /**
         * \brief The constructor
         * \param context The context of the engine
         */
        ExternComponent(states::StateContext &context);
        ExternComponent(const ExternComponent&) = delete;
        virtual ~ExternComponent();

        void create(const nlohmann::json &jsonTable) override;

        /**
         * \brief Register Lua functions for the ExternComponent class
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

    protected:
        std::string getLogErrorPrefix() const override;
    };
}