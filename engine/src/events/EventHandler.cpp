#include "SkellEngine/events/EventHandler.hpp"

#include "SkellEngine/Context.hpp"
#include "SkellEngine/utilities/json_lua.hpp"
#include "SkellEngine/states/StateManager.hpp"

namespace engine::events {
    EventHandler::EventHandler(Context &context) :
        m_context(context)
        {

    }

    EventConnection EventHandler::registerCallback(const std::string &eventType, const callbackSignature &callback, const std::string &state) {
        auto itr = m_callbacksPerEventType.find(eventType);

        if (itr == m_callbacksPerEventType.end()) {
            getContext().logger.log("Event handler: register callback: adding a new type of event: " + eventType, LogType::Info);
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
            getContext().logger.log("Event handler: impossible to send an event of type '" + type + "' because no listener were ever registered for this type");
            return false;
        }

        try {
            bool sent = itr->second.sendEvent(event);
            if (!sent) {
                getContext().logger.log("Event handler: impossible to send an event of type '" + type + "' because no listener are currently registered for this type (please check that the state given to registerCallback is correct)");
            }
            return sent;
        }
        catch(const sol::error &e) {
            getContext().logger.logError("Event handler: error during the execution of a Lua callback for the event type " + event.getType() + ":", e);
        }
        catch(const std::exception &e) {
            getContext().logger.logError("Event handler: error during the execution of a C++ callback for the event type " + event.getType() + ": ", e);
        }
        return false;
    }

    bool EventHandler::sendEvent(const std::string &type) {
        return sendEvent(type, nlohmann::json());
    }

    bool EventHandler::sendEvent(const std::string &type, const std::vector<entities::Entity::Ptr> &entities) {
        return sendEvent(type, nlohmann::json(), entities);
    }

    bool EventHandler::sendEvent(const std::string &type, const nlohmann::json &values) {
        auto event = Event::createEvent(type, getContext(), values);
        return sendEvent(*event);
    }

    bool EventHandler::sendEvent(const std::string &type, const nlohmann::json &values, const std::vector<entities::Entity::Ptr> &entities) {
        auto event = Event::createEvent(type, getContext(), values, entities);
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
                sol::resolve<bool(const std::string&)>(&EventHandler::sendEvent),
                sol::resolve<bool(const std::string&, const sol::table&)>(&EventHandler::sendEvent),
                sol::resolve<bool(const std::string&, const sol::table&, sol::variadic_args)>(&EventHandler::sendEvent)
            )
        );

        EventConnection::luaFunctions(lua);

        lua["game"]["eventHandler"] = this;
    }

    Context& EventHandler::getContext() {
        return m_context;
    }

    const Context& EventHandler::getContext() const {
        return m_context;
    }

    bool EventHandler::sendEvent(const std::string &eventType, const sol::table &table) {
        return sendEvent(eventType, utilities::lua_to_json(table));
    }

    bool EventHandler::sendEvent(const std::string &eventType, const sol::table &table, sol::variadic_args va) {
        std::vector<entities::Entity::Ptr> entities;
        entities.reserve(va.size());
        for (auto v : va) {
            if (v.is<entities::Entity::Ptr>()) {
                entities.push_back(v.get<entities::Entity::Ptr>());
            }
            else {
                getContext().logger.log("EventHandler: impossible to send an event because the list of entities contains a value that is not an entity", LogType::Error);
                return false;
            }
        }
        return this->sendEvent(eventType, utilities::lua_to_json(table), entities);
    }

    EventConnection EventHandler::registerCallbackDefaultState(const std::string &eventType, const callbackSignature &callback) {
        return registerCallback(eventType, callback, "all");
    }

    EventHandler::CallbackStorage::StoredCallback::StoredCallback(const Callback &call, const std::string &state) :
        callback(call),
        state(state) {
    }

    void EventHandler::CallbackStorage::StoredCallback::swap(EventHandler::CallbackStorage::StoredCallback &other) {
        callback.swap(other.callback);
        std::swap(state, other.state);
    }

    EventHandler::CallbackStorage::CallbackStorage(EventHandler &handler) :
        m_handler(handler) {
    }

    EventConnection EventHandler::CallbackStorage::addCallback(const Callback &callback, const std::string &state) {
        m_callbacks.emplace_front(callback, state);

        Iterator added = m_callbacks.begin();
        added->callback.setEnvironment(*this, added);

        return added->callback.shareConnection();
    }

    bool EventHandler::CallbackStorage::sendEvent(const Event &event) const {
        bool atLeastOne = false;
        for (const auto& itr : m_callbacks) {
            if (itr.state == "all" || m_handler.getContext().stateManager->isCurrentState(itr.state)) {
                itr.callback.call(event);
                atLeastOne = true;
            }
        }
        return atLeastOne;
    }

    void EventHandler::CallbackStorage::remove(EventHandler::CallbackStorage::Iterator iterator) {
        if (m_callbacks.size() != 0) {
            iterator->swap(m_callbacks.back());
            m_callbacks.pop_back();
        }
    }

    void EventHandler::CallbackStorage::clear(const std::string &state) {
        std::remove_if(m_callbacks.begin(), m_callbacks.end(), [state](const StoredCallback &callback) { return state == callback.state; });
    }

    EventHandler::CallbackConnection::CallbackConnection(const thor::Connection &connection) :
        EventConnection(connection) {
    }

    EventHandler::Callback::Callback(const callbackSignature &callback) {
        m_callback = std::make_shared<callbackSignature>(callback);
    }

    void EventHandler::Callback::call(const Event &event) const {
        if (m_callback) {
            m_callback->operator()(event);
        }
    }

    EventConnection EventHandler::Callback::shareConnection() {
        return CallbackConnection(thor::Connection(m_strongRef));
    }

    void EventHandler::Callback::swap(Callback &other) {
        std::swap(m_callback, other.m_callback);
        std::swap(m_strongRef, other.m_strongRef);
    }

    EventHandler::Callback::CallbackConnection::CallbackConnection(const thor::Connection &connection) :
        EventConnection(connection) {
    }
}