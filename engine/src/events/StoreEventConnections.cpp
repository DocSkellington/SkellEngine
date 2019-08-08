#include "SkellEngine/events/StoreEventConnections.h"

#include <typeinfo>

namespace engine::events {
    StoreEventConnections::StoreEventConnections(EventHandler &handler) :
        m_handler(handler) {

    }

    StoreEventConnections::~StoreEventConnections() {
        clearEventConnections();
    }

    EventHandler::EventConnection StoreEventConnections::registerCallback(const std::string &eventType, const EventHandler::callbackSignature &callback) {
        std::cout << callback.target_type().name() << "\n";
        // auto connection = m_handler.registerCallback(eventType, [callback](const Event &event) { callback(event); });
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
}