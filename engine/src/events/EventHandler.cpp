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

    EventHandler::EventConnection EventHandler::registerCallback(const std::string &eventType, const EventHandler::callbackSignature &callback, const std::string &state) {
        auto itr = m_callbacksPerEventType.find(eventType);

        if (itr == m_callbacksPerEventType.end()) {
            tmx::Logger::log("Event handler: register callback: adding a new type of event: " + eventType, tmx::Logger::Type::Info);
            itr = m_callbacksPerEventType.emplace(eventType, CallbackStorage(*this)).first;
        }

        return itr->second.addCallback(callback, state);
    }

    void EventHandler::clear() {
        m_callbacksPerEventType.clear();
    }

    void EventHandler::clear(const std::string &state) {
        for (auto &[_, callbackstorage] : m_callbacksPerEventType) {
            callbackstorage.clear(state);
        }
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
            tmx::Logger::log("Event handler: impossible to send an event of type '" + type + "' because no listener are currently registered for this type (please check that the state given to registerCallback is correct)");
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

    void EventHandler::luaFunctions(sol::state &lua) {
        lua.new_usertype<EventHandler>("EventHandler",
            "registerCallback", sol::overload(
                &EventHandler::registerCallback,
                &EventHandler::registerCallbackDefaultState
            ),
            "clear", sol::overload(
                sol::resolve<void()>(&EventHandler::clear),
                sol::resolve<void(const std::string&)>(&EventHandler::clear)
            ),
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

    EventHandler::EventConnection EventHandler::registerCallbackDefaultState(const std::string &eventType, const EventHandler::callbackSignature &callback) {
        return registerCallback(eventType, callback, "all");
    }

    EventHandler::CallbackStorage::Callback::Callback(const thor::detail::Listener<const Event&> &callback, const std::string &state) :
        callback(callback),
        state(state) {
    }

    void EventHandler::CallbackStorage::Callback::swap(EventHandler::CallbackStorage::Callback &other) {
        callback.swap(other.callback);
        std::swap(state, other.state);
    }

    EventHandler::CallbackStorage::CallbackConnection::CallbackConnection(const thor::Connection &connection) :
        EventConnection(connection) {
    }

    EventHandler::CallbackStorage::CallbackStorage(EventHandler &handler) :
        m_handler(handler) {
    }

    EventHandler::EventConnection EventHandler::CallbackStorage::addCallback(const EventHandler::callbackSignature &callback, const std::string &state) {
        m_callbacks.emplace_front(callback, state);

        Iterator added = m_callbacks.begin();
        added->callback.setEnvironment(*this, added);

        return CallbackConnection(added->callback.shareConnection());
    }

    bool EventHandler::CallbackStorage::sendEvent(const Event &event) const {
        bool atLeastOne = false;
        for (const auto& itr : m_callbacks) {
            if (itr.state == "all" || m_handler.getContext().stateManager->isCurrentState(itr.state)) {
                atLeastOne = true;
                itr.callback.call(event);
            }
        }
        return atLeastOne;
    }

    void EventHandler::CallbackStorage::remove(EventHandler::CallbackStorage::Iterator iterator) {
        iterator->swap(m_callbacks.front());
        m_callbacks.pop_front();
    }

    void EventHandler::CallbackStorage::clear(const std::string &state) {
        std::remove_if(m_callbacks.begin(), m_callbacks.end(), [state](const Callback &callback) { return state == callback.state; });
    }
}