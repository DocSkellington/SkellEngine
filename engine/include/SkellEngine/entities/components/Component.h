#pragma once

#include <sol/sol.hpp>
#include <nlohmann/json.hpp>

#include "SkellEngine/tmxlite/Log.hpp"
#include "SkellEngine/utilities/MemberStorage.h"

/**
* \brief Contains the definitions of the base components
*/
namespace engine::entities::components {
    /**
    * \brief The base of every component.
    * \ingroup Engine
    *
    * It defines one static function to create an instance of a component.
    *
    * If you create your own Component (in C++), you must register it before being able to use it. To do so, create a static member of type Component::RegisterComponent in .h file. In the .cpp file, simply initialise the member by passing the name of your component to the constructor of the member.
    */
    class Component : public utilities::MemberStorage {
    public:
        /**
         * \brief Every component should be manipulated through this pointer type
         */
        typedef std::shared_ptr<Component> Ptr;

    public:
        Component(Context &context);
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
        static Ptr createInstance(Context &context, const std::string &componentType);

        /**
         * \brief Register Lua functions
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

    protected:
        /**
         * \brief Structure to use when you want to register a component.
         * 
         * Create an instance of this structure in the .h file, then construct it in the .cpp file
         * \tparam The type of the component you want to register
         */
        template <typename T>
        struct RegisterComponent {
            /**
             * \brief Link the name of the component to its constructor
             * \param name The name of the component
             */
            RegisterComponent(const std::string &name) {
                static_assert(std::is_base_of<Component, T>::value, "RegisterComponent: T must inherit from Component");

                if (getMapToComponent()->find(name) != getMapToComponent()->end()) {
                    tmx::Logger::log("Component: register component: " + name + " is already defined. The value will be overwritten", tmx::Logger::Type::Warning);
                }
                getMapToComponent()->insert(std::make_pair(name, std::make_shared<T, Context&>));
            }
        };

    protected:
        virtual std::string getLogErrorPrefix() const override;
    
    private:
        /**
         * \brief The type of the map used to store the components' constructors
         */
        using MapType = std::map<std::string, std::function<Ptr(Context&)>>;

    private:

        /**
         * \brief Returns the map<string, constructor>
         */
        static std::shared_ptr<MapType> getMapToComponent();
    };
}