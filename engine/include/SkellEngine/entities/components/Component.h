/**
 * \file Component.h
 */

#pragma once

#include <sol/sol.hpp>
#include <nlohmann/json.hpp>

#include "SkellEngine/utilities/MemberStorage.h"
#include "SkellEngine/utilities/RegisterClass.h"
#include "SkellEngine/states/StateContext.h"

/**
* \brief Contains the definitions of the components directly provided by the engine
*
* Every component should inherit from Component.
* \see REGISTER_COMPONENT to easily register a new component in the engine
*/
namespace engine::entities::components {
    /**
    * \brief The base of every component.
    *
    * It defines one static function to create an instance of a component.
    *
    * If you create your own Component (in C++), you must register it before being able to use it. To do so, create a static member of type Component::RegisterComponent and initialise it.
    * \see REGISTER_COMPONENT for an helper macro to register a component
    */
    class Component : public virtual utilities::MemberStorage {
    public:
        /**
         * \brief Every component should be manipulated through this pointer type
         */
        typedef std::shared_ptr<Component> Ptr;

    public:
        Component(const Component&) = delete;
        virtual ~Component();

        /**
         * \brief Initialises a component with the given jsonTable.
         * \param jsonTable The JSON table
         */
        virtual void create(const nlohmann::json &jsonTable) = 0;

        /**
         * \brief Creates an instance of a component based on the given name. If the type is unknown, a generic component is returned.
         * 
         * The component must be registered to be known.
         * \param context The context
         * \param componentType The type of the component to create
         */
        static Ptr createInstance(states::StateContext &context, const std::string &componentType);

        /**
         * \brief Register Lua functions
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

    protected:
        /**
         * \brief The specialisation of utilities::RegisterClass for the components
         */
        using RegisteredComponents = utilities::RegisterClass<Component, states::StateContext&>;

        /**
         * \brief A shortcut to register a component
         * \tparam T The type of the component to register
         */
        template <typename T>
        using RegisterComponent = RegisteredComponents::Register<T>;

    protected:
        /**
         * \brief The constructor
         * \param context A reference to the context of the engine
         */
        Component(states::StateContext &context);

        virtual std::string getLogErrorPrefix() const override;
    };
}
    
/**
 * \brief Registers the component TYPE under the name NAME
 * 
 * \warning It must be placed <b>inside</b> of the class definition. For example, do something like:
 * \code
 * class ExampleComponent : public Component {
 *  public:
 *      ExampleComponent(StateContext& context) : MemberStorage(context.context), Component(context) { ... }
 *      ...
 *      REGISTER_COMPONENT(ExampleComponent, "example")
 * };
 * \endcode
 * 
 * \note This macro adds a private member variable. The name of the variable is the concatenation of "registeringVariable" and the line number in the header file using this macro. This allows to register multiple times the same component under different names
 * \warning This macro uses "private: ". Therefore, everything declared after this macro will be marked as private in your class definition.
 */
#define REGISTER_COMPONENT(TYPE, NAME) REGISTER_CLASS(RegisterComponent, TYPE, NAME)
