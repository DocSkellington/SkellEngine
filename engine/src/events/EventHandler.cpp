#include "SkellEngine/events/EventHandler.h"

#include "SkellEngine/tmxlite/Log.hpp"
#include "SkellEngine/Context.h"
#include "SkellEngine/utilities/json_lua.h"

namespace engine::events {
    EventHandler::EventHandler(Context &context) :
        m_context(context)
        {

    }

    int EventHandler::registerCallback(const std::string &eventType, const EventHandler::callbackSignature &callback) {
        auto itr = m_callbacksPerEventType.find(eventType);

        if (itr == m_callbacksPerEventType.end()) {
            tmx::Logger::log("Event handler: register callback: addind a new type of event: " + eventType, tmx::Logger::Type::Info);
            itr = m_callbacksPerEventType.emplace(eventType, CallbackStorage()).first;
        }

        return itr->second.addCallback(callback);
    }

    bool EventHandler::removeCallback(const std::string &eventType, int ID) {
        auto callbacks = m_callbacksPerEventType.find(eventType);
        if (callbacks == m_callbacksPerEventType.end()) {
            tmx::Logger::log("Event handler: remove a callback: unknown event type: " + eventType, tmx::Logger::Type::Warning);
            return false;
        }

        return callbacks->second.removeCallback(ID);
    }

    void EventHandler::clear() {
        m_callbacksPerEventType.clear();
    }

    bool EventHandler::sendEvent(const Event& event) const {
        const std::string& type = event.getType();
        auto itr = m_callbacksPerEventType.find(type);

        if (itr == m_callbacksPerEventType.end()) {
            tmx::Logger::log("Event handler: impossible to send an event of type " + type + " because no listener are registered for this type");
            return false;
        }

        return itr->second.sendEvent(event);
    }

    bool EventHandler::sendEvent(const std::string &type, const nlohmann::json &values) {
        auto event = Event::createEvent(type, getContext(), values);
        return sendEvent(*event);
    }

    void EventHandler::luaFunctions(sol::state &lua) const {
        lua.new_usertype<EventHandler>("EventHandler",
            "registerCallback", &EventHandler::registerCallback,
            "removeCallback", &EventHandler::removeCallback,
            "clear", &EventHandler::clear,
            "sendEvent", sol::overload(
                sol::resolve<bool(const Event&)const>(&EventHandler::sendEvent),
                sol::resolve<const std::string&, const sol::table&>(&EventHandler::sendEvent)
            )
        );

        lua["game"]["eventHandler"] = this;
    }

    Context& EventHandler::getContext() {
        return m_context;
    }

    const Context& EventHandler::getContext() const {
        return m_context;
    }

    bool EventHandler::sendEvent(const std::string &type, const sol::table &luaTable) {
        return sendEvent(type, utilities::lua_to_json(luaTable));
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

    bool EventHandler::CallbackStorage::removeCallback(int ID) {
        tmx::Logger::log("removing callback");
        if (ID < 0 || ID > m_callbacks.size()) {
            tmx::Logger::log("Event handler: invalid callback ID: " + std::to_string(ID), tmx::Logger::Type::Warning);
            return false;
        }

        if (!m_usedIDs.test(ID)) {
            tmx::Logger::log("Event handler: impossible to remove an unused callback ID: " + std::to_string(ID), tmx::Logger::Type::Warning);
            return false;
        }

        int n = m_callbacks.erase(ID);
        // The callback was removed
        if (n == 1) {
            m_usedIDs.reset(ID);
            return true;
        }
        return false;
    }

    bool EventHandler::CallbackStorage::sendEvent(const Event &event) const {
        bool atLeastOne = false;
        for (const auto& itr : m_callbacks) {
            atLeastOne = true;
            const auto& callback = itr.second;
            callback(event);
        }
        return atLeastOne;
    }
}