#pragma once

#include <any>
#include <typeinfo>
#include <typeindex>

#include <sol/sol.hpp>
#include <nlohmann/json.hpp>

#include "SkellEngine/tmxlite/Log.hpp"

namespace engine{
    struct Context;
}

/**
* \brief Contains the definitions of the base components
*/
namespace engine::entities::components {
    /**
    * \brief The base of every component.
    * \ingroup Engine
    *
    * It defines one family of functions to create the component from a lua script and/or a JSON table.
    * It also defines one static function to create an instance of a component.
    *
    * If you create, your own Component (in C++), you must register it before being able to use it. To do so, create a static member of type Component::RegisterComponent in .h file. In the .cpp file, simply initialise the member by passing the name of your component to the constructor of the member.
    */
    class Component {
    public:
        /**
         * \brief Every component should be manipulated through this pointer type
         */
        typedef std::shared_ptr<Component> Ptr;

    public:
        Component();
        Component(const Component&) = delete;
        virtual ~Component();

        /**
         * \brief Initialises a component with the given jsonTable.
         * \param jsonTable The JSON table
         */
        virtual void create(const nlohmann::json &jsonTable) = 0;

        /**
         * \brief Sets a variable called "name" to an integer value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, int value);
        /**
         * \brief Sets a variable called "name" to a long value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, long value);
        /**
         * \brief Sets a variable called "name" to a flot value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, float value);
        /**
         * \brief Sets a variable called "name" to a double value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, double value);
        /**
         * \brief Sets a variable called "name" to a boolean value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, bool value);
        /**
         * \brief Sets a variable called "name" to a const array of characters value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, const char* value);
        /**
         * \brief Sets a variable called "name" to a string value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, const std::string& value);
        /**
         * \brief Sets a variable called "name" to a JSON object
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, const nlohmann::json& value);

        /**
         * \brief Sets a variable called "name" to a Lua nil value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, sol::nil_t value);
        /**
         * \brief Sets a variable called "name" to a Lua table
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, const sol::table& value);

        /**
         * \brief Gets the value of the variable "name" as an integer
         * 
         * If the variable does not exist, the second value of the pair is false.
         * \param name The name of the variable
         * \return A pair (value, found) with value a long integer and found a boolean (true iff the variable exists)
         * \throw std::bad_any_cast When the value can not be converted to integer
         */
        virtual std::pair<long, bool> getInt(const std::string &name);
        /**
         * \brief Gets the value of the variable "name" as a floating point number
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a double and found a boolean (true iff the variable exists)
         * \throw std::bad_any_cast When the value can not be converted to double
         */
        virtual std::pair<double, bool> getFloat(const std::string &name);
        /**
         * \brief Gets the value of the variable "name" as a boolean
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a boolean and found a boolean (true iff the variable exists)
         * \throw std::bad_any_cast When the value can not be converted to boolean
         */
        virtual std::pair<bool, bool> getBool(const std::string &name);
        /**
         * \brief Gets the value of the variable "name" as a string
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a string and found a boolean (true iff the variable exists)
         * \throw std::bad_any_cast When the value can not be converted to string
         */
        virtual std::pair<std::string, bool> getString(const std::string &name);
        /**
         * \brief Gets the value of the variable "name" as a JSON object
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a JSON object and found a boolean (true iff the variable exists)
         * \throw std::bad_any_cast When the value can not be converted to a JSON object
         */
        virtual std::pair<nlohmann::json, bool> getJSON(const std::string &name);

        /**
         * \brief Gets the value of the variable "name" as a generic Lua object
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a generic Lua object and found a boolean (true iff the variable exists)
         * \throw std::bad_any_cast When the value can not be converted to a Lua object
         */
        virtual std::pair<sol::object, bool> getObject(const std::string &name);

        /**
         * \brief Creates an instance of a component based on the given name. If the type is unknown, a generic component is returned.
         * 
         * The component must be registered to be known.
         * \param context The context
         * \param componentType The type of the component to create
         */
        static Ptr createInstance(Context &context, const std::string &componentType);

        /**
         * \brief Registers the Lua functions for the Component class
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

    protected:
        /**
         * \brief The type of the map used to store the components' constructors
         */
        typedef std::map<std::string, std::function<Ptr()>> MapType;

    protected:
        /**
         * \brief Gets the context
         */
        Context& getContext();

        /**
         * \brief Sets the context of the component
         * \param context The context
         */
        void setContext(Context &context);

        /**
         * \brief Returns the map<string, constructor>
         */
        static std::shared_ptr<MapType> getMapToComponent();

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
                getMapToComponent()->insert(std::make_pair(name, std::make_shared<T>));
            }
        };

        /**
         * \brief Registers a member variable to be accessed through get and set functions
         * 
         * The name of the variable can be arbitrary set (it does not have to be the same as the variable name in the C++ class)
         * \param name The name under which to register the variable
         * \param member The pointer to the variable 
         * \see PositionComponent.cpp for an example
         */
        template <typename T>
        void registerMember(const std::string &name, T* member) {
            if (mapMembers.find(name) != mapMembers.end()) {
                tmx::Logger::log("Component: registerMember: " + name + " is already used. The component will be overwritten by the new assignation.", tmx::Logger::Type::Warning);
            }

            mapMembers.insert(std::make_pair(name, member));
        }

    private:
        Context *m_context;

        std::map<std::string, std::any> mapMembers;
    };
}