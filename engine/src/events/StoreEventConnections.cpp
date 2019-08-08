#include "SkellEngine/events/StoreEventConnections.h"

#include <typeinfo>

namespace engine::events {
    StoreEventConnections::StoreEventConnections(EventHandler &handler) :
        m_handler(handler) {

    }

    StoreEventConnections::~StoreEventConnections() {
        clearEventConnections();
    }

    EventConnection StoreEventConnections::registerCallback(const std::string &eventType, const EventHandler::callbackSignature &callback) {
        auto connection = m_handler.registerCallback(eventType, callback);
        m_connections.push_back(connection);
        return connection;
    }

    void StoreEventConnections::clearEventConnections() {
        std::cout << "Clearing connections\n";
        int i = 0;
        for (auto &connection : m_connections) {
            std::cout << i++ << "\n";
            connection.disconnect();
        }
        std::cout << "Connections cleared\n";
        m_connections.clear();
    }

    void StoreEventConnections::luaFunctions(sol::state &lua) {
        lua.set_function("registerCallback", [this](const std::string &eventType, const EventHandler::callbackSignature &callback) {
            this->registerCallback(eventType, callback);
        });
    }
}