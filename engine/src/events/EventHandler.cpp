#include "SkellEngine/events/EventHandler.h"

#include "SkellEngine/tmxlite/Log.hpp"
#include "SkellEngine/Context.h"
#include "SkellEngine/utilities/json_lua.h"

namespace engine::events {
    EventHandler::EventConnection::EventConnection(const thor::Connection &connection) :
        m_connection(connection) {

    }

    bool EventHandler::EventConnection::isConnected() const {
        return m_connection.isConnected();
    }

    void EventHandler::EventConnection::disconnect() {
        m_connection.disconnect();
    }

    EventHandler::EventHandler(Context &context) :
        m_context(context)
        {

    }

    EventHandler::EventConnection EventHandler::registerCallback(const std::string &eventType, const EventHandler::callbackSignature &callback) {
        auto itr = m_callbacksPerEventType.find(eventType);

        if (itr == m_callbacksPerEventType.end()) {
            tmx::Logger::log("Event handler: register callback: adding a new type of event: " + eventType, tmx::Logger::Type::Info);
            itr = m_callbacksPerEventType.emplace(eventType, CallbackStorage()).first;
        }

        return itr->second.addCallback(callback);
    }

    void EventHandler::clear() {
        m_callbacksPerEventType.clear();
    }

    bool EventHandler::sendEvent(const Event& event) const {
        const std::string& type = event.getType();
        auto itr = m_callbacksPerEventType.find(type);

        if (itr == m_callbacksPerEventType.end()) {
            tmx::Logger::log("Event handler: impossible to send an event of type '" + type + "' because no listener were ever registered for this type");
            return false;
        }

        bool sent = itr->second.sendEvent(event);
        if (!sent) {
            tmx::Logger::log("Event handler: impossible to send an event of type '" + type + "' because no listener are currently registered for this type");
        }
        return sent;
    }

    bool EventHandler::sendEvent(const std::string &type) {
        return sendEvent(type, nlohmann::json());
    }

    bool EventHandler::sendEvent(const std::string &type, const nlohmann::json &values) {
        auto event = Event::createEvent(type, getContext(), values);
        return sendEvent(*event);
    }

    void EventHandler::luaFunctions(sol::state &lua) const {
        lua.new_usertype<EventHandler>("EventHandler",
            "registerCallback", &EventHandler::registerCallback,
            "clear", &EventHandler::clear,
            "sendEvent", sol::overload(
                sol::resolve<bool(const Event&)const>(&EventHandler::sendEvent),
                sol::resolve<const std::string&, const sol::table&>(&EventHandler::sendEvent)
            )
        );

        lua.new_usertype<EventHandler::EventConnection>("EventConnection",
            "isConnected", &EventHandler::EventConnection::isConnected,
            "disconnect", &EventHandler::EventConnection::disconnect
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

    EventHandler::CallbackStorage::Callback::Callback(const EventHandler::callbackSignature &callback) :
        m_callback(callback) {

    }

    void EventHandler::CallbackStorage::Callback::call(const Event &event) const {
        m_callback(event);
    }

    void EventHandler::CallbackStorage::Callback::setEnvironment(CallbackStorage &container, CallbackStorage::Iterator iterator) {
        m_strongRef = thor::detail::makeIteratorConnectionImpl(container, iterator);
    }

    EventHandler::EventConnection EventHandler::CallbackStorage::Callback::shareConnection() const {
        return EventHandler::EventConnection(thor::Connection(m_strongRef));
    }

    void EventHandler::CallbackStorage::Callback::swap(EventHandler::CallbackStorage::Callback &other) {
        std::swap(m_callback, other.m_callback);
        std::swap(m_strongRef, other.m_strongRef);
    }

    EventHandler::EventConnection EventHandler::CallbackStorage::addCallback(const EventHandler::callbackSignature &callback) {
        m_callbacks.push_front(callback);

        Iterator added = m_callbacks.begin();
        added->setEnvironment(*this, added);

        return added->shareConnection();
    }

    bool EventHandler::CallbackStorage::sendEvent(const Event &event) const {
        bool atLeastOne = false;
        for (const auto& itr : m_callbacks) {
            atLeastOne = true;
            itr.call(event);
        }
        return atLeastOne;
    }

    void EventHandler::CallbackStorage::remove(EventHandler::CallbackStorage::Iterator iterator) {
        iterator->swap(m_callbacks.front());
        m_callbacks.pop_front();
    }
}