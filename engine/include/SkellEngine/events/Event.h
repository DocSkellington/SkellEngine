#pragma once

#include <string>
#include <memory>
#include <functional>

#include <sol/sol.hpp>

#include "SkellEngine/utilities/MemberStorage.h"

namespace engine::events {
    /**
     * \brief The base class for events
     */
    class Event : public utilities::MemberStorage {
    public:
        typedef std::shared_ptr<Event> Ptr;

    public:
        /**
         * \brief Constructs an event
         * \param type The type of the event
         */
        explicit Event(const std::string &type);
        Event(const Event&) = delete;
        virtual ~Event();

        /**
         * \brief Get the type of the event
         * \param The type of the event
         */
        const std::string &getType() const noexcept;

        static void luaFunctions(sol::state &lua);

    protected:
        /**
         * \brief The structure to use to register a new event class
         */
        template<typename T>
        struct RegisterEvent {
            RegisterEvent(const std::string &name) {
                if (getMapToEvents()->find(name) != getMapToEvents()->end()) {
                    tmx::Logger::log("Event: register event: " + name + " is already defined. The value will be overwritten", tmx::Logger::Type::Warning);
                }
                getMapToEvents()->emplace(name, std::make_shared<T>);
            }
        };

    private:
        using mapType = std::map<std::string, std::function<Ptr()>>;

    private:
        static std::shared_ptr<mapType> getMapToEvents();
    
    private:
        const std::string m_type;
    };
}

// TODO: ExternalEvent (once Lua interface with systems is done)