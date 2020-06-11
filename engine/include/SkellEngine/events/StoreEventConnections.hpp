#pragma once

#include "SkellEngine/events/EventConnection.hpp"
#include "SkellEngine/events/EventHandler.hpp"

namespace engine::events {
    /**
     * \brief Allows to store EventConnections
     */
    class StoreEventConnections {
    public:
        /**
         * \brief The constructor
         * \param handler The EventHandler (needed to register a new callback)
         */
        StoreEventConnections(EventHandler &handler);
        virtual ~StoreEventConnections();

        /**
         * \brief Registers a new callback in the event handler
         * \param eventType The type of the event to listen to
         * \param callback The callback to add
         * \param state The state of the game in which the callback is active. If "all", the callback is active in every state
         * \return An connection to the registered callback.
         * \see systems::System::registerCallback for systems
         * \see EventHandler::registerCallback if you do not want to use this class
         */
        virtual EventConnection registerCallback(const std::string &eventType, const EventHandler::callbackSignature &callback, const std::string &state = "all");

        /**
         * \brief Adds directly a connection to a registered callback
         * 
         * It does not create a new callback
         * \param connection The connection
         */
        virtual void addEventConnection(const EventConnection &connection);

        /**
         * \brief Disconnects every event connection stored and clears the internal storage
         */
        virtual void clearEventConnections();

        /**
         * \brief Registers a "registerCallback" Lua function
         * \param lua The Lua state
         */
        virtual void luaFunctions(sol::state &lua);

    private:
        using Container = std::list<EventConnection>;

    private:
        Container m_connections;
        EventHandler &m_handler;
    };
}