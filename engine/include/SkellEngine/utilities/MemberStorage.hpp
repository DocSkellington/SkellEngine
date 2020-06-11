#pragma once

#include <string>
#include <any>
#include <map>

#include <sol/sol.hpp>

#include <nlohmann/json.hpp>

#include "SkellEngine/errors/WrongType.hpp"
#include "SkellEngine/Context.hpp"

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
        virtual void set(const std::string &name, int value) noexcept;
        /**
         * \brief Sets a variable called "name" to a long value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, long value) noexcept;
        /**
         * \brief Sets a variable called "name" to a flot value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, float value) noexcept;
        /**
         * \brief Sets a variable called "name" to a double value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, double value) noexcept;
        /**
         * \brief Sets a variable called "name" to a boolean value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, bool value) noexcept;
        /**
         * \brief Sets a variable called "name" to a const array of characters value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, const char* value) noexcept;
        /**
         * \brief Sets a variable called "name" to a string value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, const std::string& value) noexcept;
        /**
         * \brief Sets a variable called "name" to a JSON object
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, const nlohmann::json& value) noexcept;

        /**
         * \brief Sets a variable called "name" to a Lua nil value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, sol::nil_t value) noexcept;
        /**
         * \brief Sets a variable called "name" to a Lua table
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        virtual void set(const std::string &name, const sol::table& value) noexcept;

        /**
         * \brief Checks if a variable called "name" exists
         * \param name The name of the variable
         * \return True iff the variable exists
         */
        virtual bool has(const std::string &name) const noexcept;

        /**
         * \brief Gets the value of the variable "name" as an integer
         * 
         * If the variable does not exist, the second value of the pair is false.
         * \param name The name of the variable
         * \return A pair (value, found) with value a long integer and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to integer
         */
        virtual std::pair<long, bool> getInt(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a floating point number
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a double and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to double
         */
        virtual std::pair<double, bool> getFloat(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a boolean
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a boolean and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to boolean
         */
        virtual std::pair<bool, bool> getBool(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a string
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a string and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to string
         */
        virtual std::pair<std::string, bool> getString(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a JSON object
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a JSON object and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a JSON object
         */
        virtual std::pair<nlohmann::json, bool> getJSON(const std::string &name) const;

        /**
         * \brief Gets the value of the variable "name" as a generic Lua object
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a generic Lua object and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a Lua object
         */
        virtual std::pair<sol::object, bool> getObject(const std::string &name) const;

        /**
         * \brief Registers the Lua functions for the MemberStorage class
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

    protected:
        /**
         * \brief The internal container
         */
        using Container = std::map<std::string, std::any>;

    protected:
        /**
         * \brief Writes the value of given type in a std::any variable
         * \param variable The variable to modify
         * \param value The value to write
         * \tparam T The type of the variable
         */
        template <typename T>
        void convertAndSet(std::any variable, T value) noexcept {
            T *va = std::any_cast<T*>(variable);
            *va = value;
        }

        /**
         * \brief Gets the value of a std::any variable as the type T
         * \param variable The variable to read
         * \return The value cast to T
         * \throw errors::WrongType when the value can not be converted to T
         * \tparam T The type of the variable
         */
        template <typename T>
        T getValue(std::any variable) const {
            try {
                T *va = std::any_cast<T*>(variable);
                return *va;
            }
            catch (const std::bad_any_cast &e) {
                throw errors::WrongType(e.what());
            }
        }

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
                m_context.logger.log(getLogErrorPrefix() + ": registerMember: " + name + " is already used. The component will be overwritten by the new assignation.", LogType::Warning);
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

        /** @{ */
        /**
         * \brief Gets the internal container
         * \return The internal container
         */
        Container& getContainer();
        const Container& getContainer() const;
        /** @} */

    private:
        Container mapMembers;

        Context &m_context;
    };
}