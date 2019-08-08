#pragma once

#include "SkellEngine/events/EventHandler.h"

namespace engine::events {
    class StoreEventConnections {
    public:
        StoreEventConnections(EventHandler &handler);
        virtual ~StoreEventConnections();

        virtual EventHandler::EventConnection registerCallback(const std::string &eventType, const EventHandler::callbackSignature &callback);

        virtual void clearEventConnections();

    private:
        using Container = std::list<EventHandler::EventConnection>;

    private:
        Container m_connections;
        EventHandler &m_handler;
    };
}