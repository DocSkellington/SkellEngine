#include "SkellEngine/events/EventConnection.h"

namespace engine::events {
    EventConnection::EventConnection(const thor::Connection &connection) :
        m_connection(connection) {

    }

    bool EventConnection::isConnected() const {
        return m_connection.isConnected();
    }

    void EventConnection::disconnect() {
        m_connection.disconnect();
    }

    void EventConnection::luaFunctions(sol::state &lua) {
        lua.new_usertype<EventConnection>("EventConnection",
            "isConnected", &EventConnection::isConnected,
            "disconnect", &EventConnection::disconnect
        );
    }
}