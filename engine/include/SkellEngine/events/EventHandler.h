#pragma once

#include <map>
#include <list>
#include <string>
#include <memory>
#include <functional>
#include <bitset>

#include "SkellEngine/events/Event.h"

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
        EventHandler();
        EventHandler(const EventHandler&) = delete;

        /**
         * \brief Add a callback (a listener) for a specific event type
         * 
         * If the event type is unkown, the listener is still registered. So, make sure to write the type correctly.
         * \param eventType The type of the event to listen to
         * \param callback The callback to add
         * \return -1 if the callback could not be added or the ID of the callback
         */
        int registerCallback(const std::string &eventType, const callbackSignature &callback);

        /**
         * \brief Remove a callback from the list associated with an event type
         * \param eventType The type of the event
         * \param callback The ID of the callback to remove
         * \return True iff the callback was removed
         */
        bool removeCallback(const std::string &eventType, int ID);

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

    private:
        class CallbackStorage {
        public:
            int addCallback(const callbackSignature &callback);

            bool removeCallback(int ID);

            bool sendEvent(const Event& event) const;

        private:
            std::bitset<sizeof(int) / 2> m_usedIDs;

            std::map<int, callbackSignature> m_callbacks;
        };

    private:
        std::map<std::string, CallbackStorage> m_callbacksPerEventType;
    };
}