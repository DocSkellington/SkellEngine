#pragma once

#include <map>
#include <list>
#include <string>
#include <memory>
#include <functional>
#include <bitset>

#include <Thor/Input/Connection.hpp>

#include "SkellEngine/events/Event.h"

namespace engine {
    struct Context;
}

namespace engine::events {
    /**
     * \brief Handles events in the engine
     * 
     * Each event has a type and the handler maps a type with a list of callbacks.
     * When an event is sent, the callbacks associated with this event type are called.
     */
    class EventHandler final {
    public:
        /**
         * \brief The signature of a callback function
         * 
         * You can cast the event into the specific class or use the generic getters and setters
         */
        using callbackSignature = std::function<void(const Event&)>;

        /**
         * \brief Allows to remove a callback from the event handler
         * 
         * If the connection object dies (the variable reaches the end of its scope), the actual connection is <b>not</b> removed (the callback will still be called when an appropriate input is triggered).
         */
        class EventConnection {
        public:
            /**
             * \brief The constructor
             * 
             * It should not be called by the user (but it was impossible to hide it)
             * \param connection The connection
             */
            EventConnection(const thor::Connection &connection);

            /**
             * \brief TODO:
             */
            bool isConnected() const;
            void disconnect();

        private:
            thor::Connection m_connection;
        };

    public:
        EventHandler(Context &context);
        EventHandler(const EventHandler&) = delete;

        /**
         * \brief Add a callback (a listener) for a specific event type
         * 
         * If the event type is unkown, the listener is still registered. So, make sure to write the type correctly.
         * \param eventType The type of the event to listen to
         * \param callback The callback to add
         * \return -1 if the callback could not be added or the ID of the callback
         */
        EventConnection registerCallback(const std::string &eventType, const callbackSignature &callback);

        /**
         * \brief Remove every callback
         */
        void clear();

        /**
         * \brief Send an event
         * 
         * The event type is deduced from the given event
         * \param event The event to send
         * \return True iff the event was sent to at least one receiver
         */
        bool sendEvent(const events::Event &event) const;

        /**
         * \brief Construct and send an empty event (with just the type)
         * \param type The type of the event to send
         * \return True iff the event was sent to at least one receiver
         */
        bool sendEvent(const std::string &type); // is not const because of the ExternalEvent implementation

        /**
         * \brief Construct and send an event
         * \param type The type of the event to send
         * \param values The values to set in the event
         * \return True iff the event was sent to at least one receiver
         */
        bool sendEvent(const std::string &type, const nlohmann::json &values); // is not const because of the ExternalEvent implementation

        /**
         * \brief Register the Lua functions
         * \param lua The Lua state
         */
        void luaFunctions(sol::state &lua) const;

    private:

        class CallbackStorage {
        public:
            class Callback;
            using Container = std::list<Callback>;
            using Iterator = Container::iterator;

            /**
             * \brief A callback
             * 
             * This class is inspired from thor::detail::Listener
             */
            class Callback {
            public:
                Callback(const callbackSignature &callback);

                void call(const Event &event) const;

                void setEnvironment(CallbackStorage &container, CallbackStorage::Iterator iterator);

                EventConnection shareConnection() const;

                void swap(Callback &other);

            private:
                callbackSignature m_callback;
                std::shared_ptr<thor::detail::AbstractConnectionImpl> m_strongRef;
            };

        public:
            EventConnection addCallback(const callbackSignature &callback);

            bool sendEvent(const Event& event) const;

            void remove(Iterator iterator);

        private:
            Container m_callbacks;
        };

    private:
        Context& getContext();
        const Context& getContext() const;

        bool sendEvent(const std::string &type, const sol::table &luaTable);

    private:
        std::map<std::string, CallbackStorage> m_callbacksPerEventType;
        Context &m_context;
    };
}