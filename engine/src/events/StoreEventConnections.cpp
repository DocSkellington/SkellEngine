#include "SkellEngine/events/StoreEventConnections.hpp"

#include <typeinfo>

namespace engine::events {
    StoreEventConnections::StoreEventConnections(EventHandler &handler) :
        m_handler(handler) {

    }

    StoreEventConnections::~StoreEventConnections() {
        clearEventConnections();
    }

    EventConnection StoreEventConnections::registerCallback(const std::string &eventType, const EventHandler::callbackSignature &callback, const std::string &state) {
        auto connection = m_handler.registerCallback(eventType, callback, state);
        m_connections.push_back(connection);
        return connection;
    }

    void StoreEventConnections::addEventConnection(const EventConnection &connection) {
        m_connections.push_back(connection);
    }

    void StoreEventConnections::clearEventConnections() {
        for (auto &connection : m_connections) {
            connection.disconnect();
        }
        m_connections.clear();
    }

    void StoreEventConnections::luaFunctions(sol::state &lua) {
        lua.set_function("registerCallback", [this](const std::string &eventType, const EventHandler::callbackSignature &callback) {
            this->registerCallback(eventType, callback);
        });
    }
}