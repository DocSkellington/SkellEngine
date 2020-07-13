#pragma once

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "SkellEngine/events/Event.hpp"

namespace engine::events {
    /**
     * \brief This event is defined only with JSON
     * 
     * It can be modified in both Lua and C++
     * 
     * When no registered event matches the required event type, an extern event is created. It can be used as any other event
     */
    class ExternEvent : public Event {
    public:
        /**
         * \brief The constructor
         * \param type The type of the event
         * \param context The context of the engine
         */
        ExternEvent(const std::string &type, Context &context);

        void create(const nlohmann::json &jsonTable) override;

        /**
         * \brief Register Lua functions for the ExternEvent class
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

    protected:
        virtual std::string getLogErrorPrefix() const override;
    };
}
