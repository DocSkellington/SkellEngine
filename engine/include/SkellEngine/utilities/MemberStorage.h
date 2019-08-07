#pragma once

#include <string>
#include <any>
#include <map>

#include <sol/sol.hpp>

#include <nlohmann/json.hpp>

#include "SkellEngine/tmxlite/Log.hpp"

namespace engine{
    struct Context;
}

namespace engine::utilities {
    /**
     * \brief Allows to retrieve and modify the registered members of a class
     * 
     * Once variables are registered, generic getters and setters can be used.
     * \see registerMember for registering a variable member
     */
    class MemberStorage {
    public:
        /**
         * \brief The constructor
         * \param context A reference to the context of the engine
         */
        MemberStorage(Context &context);
        virtual ~MemberStorage();

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
        virtual std::pair<long, bool> getInt(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a floating point number
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a double and found a boolean (true iff the variable exists)
         * \throw std::bad_any_cast When the value can not be converted to double
         */
        virtual std::pair<double, bool> getFloat(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a boolean
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a boolean and found a boolean (true iff the variable exists)
         * \throw std::bad_any_cast When the value can not be converted to boolean
         */
        virtual std::pair<bool, bool> getBool(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a string
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a string and found a boolean (true iff the variable exists)
         * \throw std::bad_any_cast When the value can not be converted to string
         */
        virtual std::pair<std::string, bool> getString(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a JSON object
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a JSON object and found a boolean (true iff the variable exists)
         * \throw std::bad_any_cast When the value can not be converted to a JSON object
         */
        virtual std::pair<nlohmann::json, bool> getJSON(const std::string &name) const;

        /**
         * \brief Gets the value of the variable "name" as a generic Lua object
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a generic Lua object and found a boolean (true iff the variable exists)
         * \throw std::bad_any_cast When the value can not be converted to a Lua object
         */
        virtual std::pair<sol::object, bool> getObject(const std::string &name) const;

        /**
         * \brief Registers the Lua functions for the MemberStorage class
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

    protected:
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
                tmx::Logger::log(getLogErrorPrefix() + ": registerMember: " + name + " is already used. The component will be overwritten by the new assignation.", tmx::Logger::Type::Warning);
            }

            mapMembers.insert(std::make_pair(name, member));
        }

        /**
         * \brief Gets the context
         */
        Context& getContext();
        /**
         * \brief Gets the context
         */
        const Context& getContext() const;

        /**
         * \brief What to print in front of the log messages
         * 
         * For example, the class Component returns "Component"
         * \return The string to print
         * \see Implementation of registerMember
         */
        virtual std::string getLogErrorPrefix() const = 0;

    private:
        std::map<std::string, std::any> mapMembers;

        Context &m_context;
    };
}