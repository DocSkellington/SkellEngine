#pragma once

#include <string>
#include <memory>
#include <functional>

#include <sol/sol.hpp>

#include "SkellEngine/utilities/MemberStorage.h"
#include "SkellEngine/utilities/RegisterClass.h"

namespace engine::events {
    /**
     * \brief The base class for events
     * 
     * It defines one static function to create an instance of an event
     * 
     * If you create your own Event (in C++), you must register it before being able to use it. To do so, create a static member of type Event::
     * \see REGISTER_EVENT for an helper macro to register a component
     */
    class Event : public utilities::MemberStorage {
    public:
        /**
         * \brief Every event should be manipulated through this pointer type
         */
        typedef std::shared_ptr<Event> Ptr;

    public:
        Event(const Event&) = delete;
        virtual ~Event();

        /**
         * \brief Initialises an event with the given JSON table
         * \param jsonTable The JSON table
         */
        virtual void create(const nlohmann::json &jsonTable) = 0;

        /**
         * \brief Get the type of the event
         * \param The type of the event
         */
        const std::string &getType() const noexcept;

        /**
         * \brief Register Lua functions
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

        /**
         * \brief Create an event of given type
         * \param type The type
         * \return A shared pointer to the created event
         */
        static Ptr createEvent(const std::string &type, Context &context);

        /**
         * \brief Create an event of given type and immediately sets variables defined in the JSON table
         * \param type The type
         * \param context The context
         * \param jsonTable The JSON table containing the variables to set in the event
         * \return A shared pointer to the created event
         */
        static Ptr createEvent(const std::string &type, Context &context, const nlohmann::json &jsonTable);

    protected:
        /**
         * \brief The specialisation of utilities::RegisterClass for the events
         */
        using RegisteredEvents = utilities::RegisterClass<Event, Context&>;

        /**
         * \brief A shortcut to register an event
         * \tparam T The type of the event to register
         */
        template <typename T>
        using RegisterEvent = RegisteredEvents::Register<T>;

    protected:
        /**
         * \brief Constructs an event
         * \param context The context of the engine
         * \param type The type of the event
         */
        explicit Event(Context &context, const std::string &type);

        virtual std::string getLogErrorPrefix() const override;

    private:
        const std::string m_type;
    };
}

/**
 * \brief Registers the event TYPE under the name NAME
 * 
 * \warning It must be placed <b>inside</b> of the class definition. For example, do something like:
 * \code
 * class ExampleEvent : public Event {
 *  public:
 *      ExampleEvent(Context& context) : Event(context) { ... }
 *      ...
 *      REGISTER_EVENT(ExampleEvent, "example")
 * };
 * \endcode
 * 
 * \note This macro adds a private member variable. The name of the variable is the concatenation of "registeringVariable" and the line number in the header file using this macro. This allows to register multiple times the same event under different names
 * \warning This macro uses "private: ". Therefore, everything declared after this macro will be marked as private in your class definition.
 */
#define REGISTER_EVENT(TYPE, NAME) REGISTER_CLASS(RegisterEvent, TYPE, NAME)