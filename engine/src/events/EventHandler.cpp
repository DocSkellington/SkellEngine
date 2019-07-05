#include "SkellEngine/events/EventHandler.h"

#include "SkellEngine/tmxlite/Log.hpp"

namespace engine::events {
    EventHandler::EventHandler() {

    }

    int EventHandler::registerCallback(const std::string &eventType, const EventHandler::callbackSignature &callback) {
        auto itr = m_callbacksPerEventType.find(eventType);

        if (itr == m_callbacksPerEventType.end()) {
            itr = m_callbacksPerEventType.emplace(eventType, CallbackStorage()).first;
        }

        return itr->second.addCallback(callback);
    }

    void EventHandler::removeCallback(const std::string &eventType, int ID) {
        auto callbacks = m_callbacksPerEventType.find(eventType);
        if (callbacks == m_callbacksPerEventType.end()) {
            tmx::Logger::log("Event handler: remove a callback: unknown event type", tmx::Logger::Type::Warning);
            return;
        }

        m_callbacksPerEventType.at(eventType).removeCallback(ID);
    }

    void EventHandler::sendEvent(const Event& event) const {
        const std::string& type = event.getType();
        auto itr = m_callbacksPerEventType.find(type);

        if (itr == m_callbacksPerEventType.end()) {
            tmx::Logger::log("Event handler: impossible to send an event of type " + type + " because no listener are registered for this type");
            return;
        }

        itr->second.sendEvent(event);
    }

    int EventHandler::CallbackStorage::addCallback(const EventHandler::callbackSignature &callback) {
        int ID = -1;

        // We seek the first available ID
        for (std::size_t i = 0 ; i < m_usedIDs.size() ; i++) {
            if (!m_usedIDs.test(i)) {
                ID = i;
                break;
            }
        }

        if (ID == -1) {
            return -1;
        }

        m_callbacks[ID] = callback;
        m_usedIDs.set(ID);
        return ID;
    }

    void EventHandler::CallbackStorage::removeCallback(int ID) {
        if (ID < 0 || ID > m_callbacks.size()) {
            tmx::Logger::log("Event handler: invalid callback ID: " + std::to_string(ID), tmx::Logger::Type::Warning);
            return;
        }

        if (!m_usedIDs.test(ID)) {
            tmx::Logger::log("Event handler: impossible to remove an unused callback ID: " + std::to_string(ID), tmx::Logger::Type::Warning);
            return;
        }

        m_callbacks.erase(ID);
        m_usedIDs.reset(ID);
    }

    void EventHandler::CallbackStorage::sendEvent(const Event &event) const {
        for (const auto& itr : m_callbacks) {
            const auto& callback = itr.second;
            callback(event);
        }
    }
}