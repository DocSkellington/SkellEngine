#pragma once

#include "SkellEngine/events/EventConnection.h"
#include "SkellEngine/events/EventHandler.h"

namespace engine::events {
    class StoreEventConnections {
    public:
        StoreEventConnections(EventHandler &handler);
        virtual ~StoreEventConnections();

        virtual EventConnection registerCallback(const std::string &eventType, const EventHandler::callbackSignature &callback);

        virtual void clearEventConnections();

        virtual void luaFunctions(sol::state &lua);

    private:
        using Container = std::list<EventConnection>;

    private:
        Container m_connections;
        EventHandler &m_handler;
    };
}