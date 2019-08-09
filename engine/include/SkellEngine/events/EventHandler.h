#pragma once

#include <map>
#include <list>
#include <string>
#include <memory>
#include <functional>
#include <bitset>

#include <Thor/Input/Connection.hpp>
#include <Thor/Input/Detail/EventListener.hpp>

#include "SkellEngine/events/Event.h"
#include "SkellEngine/events/EventConnection.h"

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
    public:
        /**
         * \brief The constructor
         * \param context A reference to the context of the engine
         */
        EventHandler(Context &context);
        EventHandler(const EventHandler&) = delete;

        /**
         * \brief Add a callback (a listener) for a specific event type
         * 
         * If the event type is unkown, the listener is still registered. So, make sure to write the type correctly.
         * \param eventType The type of the event to listen to
         * \param callback The callback to add
         * \param state The state of the game in which the callback is active. If "all", the callback is active in every state
         * \return A connection to the registered callback. If the callback could not be registered, the connection is invalid
         */
        EventConnection registerCallback(const std::string &eventType, const callbackSignature &callback, const std::string &state = "all");

        /**
         * \brief Remove every callback
         */
        void clear();

        /**
         * \brief Remove every callback tied to the given state
         * \param state The state of the callbacks to remove
         */
        void clear(const std::string &state);

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
        void luaFunctions(sol::state &lua);

    private:
        using Callback = thor::detail::Listener<const Event&>;

        /**
         * \brief A constructible implementation of EventConnection
         */
        class CallbackConnection : public EventConnection {
        public:
            /**
             * \brief The constructor
             * \param connection The thor::Connection to the registered callback
             */
            CallbackConnection(const thor::Connection &connection);
        };

        /**
         * \brief How the callbacks are stored
         */
        class CallbackStorage {
        public:
            /**
             * \brief Stores the callback and the state in which the callback is active
             */
            struct StoredCallback {
                /**
                 * \brief Constructor
                 * \param callback The callback
                 * \param state The state in which the state is active
                 */
                StoredCallback(const Callback &callback, const std::string &state);
                
                /**
                 * \brief Swaps this callback with an other callback
                 * \param other The other callback
                 */
                void swap(StoredCallback &other);

                /**
                 * \brief The callback
                 */
                Callback callback;

                /**
                 * \brief The state in which the callback is active
                 */
                std::string state;
            };

            /**
             * \brief How the callbacks are effectively stored
             */
            using Container = std::list<StoredCallback>;
            /**
             * \brief Defines the iterator on the container
             * 
             * Necessary for thor::detail::Listener
             */
            using Iterator = Container::iterator;

        public:
            /**
             * \brief Constructor
             * \param handler A reference to the event handler
             */
            CallbackStorage(EventHandler &handler);

            /**
             * \brief Adds a new callback to this storage
             * \param callback The callback
             * \return A connection to the registered callback
             */
            EventConnection addCallback(const Callback &callback, const std::string &state);

            /**
             * \brief Calls every callback with the given event
             * \param event The event to send
             * \return True iff at least one callback was called
             */
            bool sendEvent(const Event& event) const;

            /**
             * \brief Removes a callback
             * \param iterator The iterator to the callback to remove
             * \see EventConnection
             */
            void remove(Iterator iterator);

            /**
             * \brief Removes every callback tied to the given state
             * \param state The state of the callbacks to remove
             */
            void clear(const std::string &state);

        private:
            Container m_callbacks;
            EventHandler &m_handler;
        };

    private:
        Context& getContext();
        const Context& getContext() const;

        bool sendEvent(const std::string &type, const sol::table &luaTable);
        /**
         * \brief Used for easily overloading registerCallback
         * 
         * It sets the state of the callback to "all"
         * \param eventType The event type
         * \param callback The callback
         * \todo TODO: add in documentation that in the init function, the state is not yet changed
         */
        EventConnection registerCallbackDefaultState(const std::string &eventType, const callbackSignature &callback);

    private:
        std::map<std::string, CallbackStorage> m_callbacksPerEventType;
        Context &m_context;
    };
}