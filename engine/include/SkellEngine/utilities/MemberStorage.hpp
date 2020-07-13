#pragma once

#include <string>
#include <any>
#include <map>

#include <sol/sol.hpp>

#include <nlohmann/json.hpp>

#include "SkellEngine/errors/WrongType.hpp"
#include "SkellEngine/Context.hpp"
#include "SkellEngine/math/Vector2.hpp"

namespace engine::utilities {
    /**
     * \brief Allows to automatically define getters and setters for variables.
     * 
     * The variables can be class' members or "free" (i.e., not member).
     * If a variable is a member, then the setters directly modify the member.
     * 
     * A free member is not bound to a specific type. In other words, a free variable can be a string at one moment and, later, a Vector2.
     * 
     * If a value is set but is not yet known, then it is considered free.
     * If, later on, the free variable becomes a member variable, the free variable is erased.
     * 
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
         * \brief Sets a variable called "name" to an std::any value.
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, std::any value) noexcept;
        /**
         * \brief Sets a variable called "name" to a char value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, char value) noexcept;
        /**
         * \brief Sets a variable called "name" to an unsigned char value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, unsigned char value) noexcept;
        /**
         * \brief Sets a variable called "name" to an integer value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, int value) noexcept;
        /**
         * \brief Sets a variable called "name" to an unsigned int value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, unsigned int value) noexcept;
        /**
         * \brief Sets a variable called "name" to a long value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, long value) noexcept;
        /**
         * \brief Sets a variable called "name" to an unsigned long value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, unsigned long value) noexcept;
        /**
         * \brief Sets a variable called "name" to a float value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, float value) noexcept;
        /**
         * \brief Sets a variable called "name" to a double value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, double value) noexcept;
        /**
         * \brief Sets a variable called "name" to a boolean value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, bool value) noexcept;
        /**
         * \brief Sets a variable called "name" to a const array of characters value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const char* value) noexcept;
        /**
         * \brief Sets a variable called "name" to a string value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const std::string& value) noexcept;
        /**
         * \brief Sets a variable called "name" to a JSON object
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const nlohmann::json& value) noexcept;

        /**
         * \brief Sets a variable called "name" to a Lua nil value
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, sol::nil_t value) noexcept;
        /**
         * \brief Sets a variable called "name" to a Lua table
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const sol::table& value) noexcept;
        /**
         * \brief Sets a variable called "name" to a Vector2c table
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const Vector2c &value) noexcept;
        /**
         * \brief Sets a variable called "name" to a Vector2uc table
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const Vector2uc &value) noexcept;
        /**
         * \brief Sets a variable called "name" to a Vector2i table
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const Vector2i &value) noexcept;
        /**
         * \brief Sets a variable called "name" to a Vector2ui table
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const Vector2ui &value) noexcept;
        /**
         * \brief Sets a variable called "name" to a Vector2l table
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const Vector2l &value) noexcept;
        /**
         * \brief Sets a variable called "name" to a Vector2ul table
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const Vector2ul &value) noexcept;
        /**
         * \brief Sets a variable called "name" to a Vector2f table
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const Vector2f &value) noexcept;
        /**
         * \brief Sets a variable called "name" to a Vector2d table
         * \param name The name of the variable. If the variable already exists, its value is overwritten
         * \param value The value
         */
        void set(const std::string &name, const Vector2d &value) noexcept;

        /**
         * \brief Checks if a variable called "name" exists
         * \param name The name of the variable
         * \return True iff the variable exists
         */
        bool has(const std::string &name) const noexcept;

        /**
         * \brief Gets the value of the variable "name" directly as an std::any object
         * 
         * If the variable does not exist, the second value of the pair is false.
         * \param name The name of the variable
         * \return A pair (value, found) with value an std::any and found a boolean (true iff the variable exists)
         */
        std::pair<std::any, bool> getAny(const std::string &name) const noexcept;

        /**
         * \brief Gets the value of the variable "name" as a char
         * 
         * If the variable does not exist, the second value of the pair is false.
         * \param name The name of the variable
         * \return A pair (value, found) with value a char and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to integer
         */
        std::pair<char, bool> getChar(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as an unsigned char
         * 
         * If the variable does not exist, the second value of the pair is false.
         * \param name The name of the variable
         * \return A pair (value, found) with value an unsigned char and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to integer
         */
        std::pair<unsigned char, bool> getUnsignedChar(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as an integer
         * 
         * If the variable does not exist, the second value of the pair is false.
         * \param name The name of the variable
         * \return A pair (value, found) with value a long integer and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to integer
         */
        std::pair<int, bool> getInt(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as an unsigned integer
         * 
         * If the variable does not exist, the second value of the pair is false.
         * \param name The name of the variable
         * \return A pair (value, found) with value an unsigned integer and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to integer
         */
        std::pair<unsigned int, bool> getUnsignedInt(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a long
         * 
         * If the variable does not exist, the second value of the pair is false.
         * \param name The name of the variable
         * \return A pair (value, found) with value a long integer and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to integer
         */
        std::pair<long, bool> getLong(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as an unsigned long
         * 
         * If the variable does not exist, the second value of the pair is false.
         * \param name The name of the variable
         * \return A pair (value, found) with value an unsigned long integer and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to integer
         */
        std::pair<unsigned long, bool> getUnsignedLong(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a single-precision floating point number
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a float and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to double
         */
        std::pair<float, bool> getFloat(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a double-precision floating point number
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a double and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to double
         */
        std::pair<double, bool> getDouble(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a boolean
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a boolean and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to boolean
         */
        std::pair<bool, bool> getBool(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a string
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a string and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to string
         */
        std::pair<std::string, bool> getString(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a JSON object
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a JSON object and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a JSON object
         */
        std::pair<nlohmann::json, bool> getJSON(const std::string &name) const;

        /**
         * \brief Gets the value of the variable "name" as a generic Lua object
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a generic Lua object and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a Lua object
         */
        std::pair<sol::object, bool> getObject(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a Vector2c
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a Vector2c and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a Lua object
         */
        std::pair<Vector2c, bool> getVector2c(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a Vector2uc
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a Vector2uc and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a Lua object
         */
        std::pair<Vector2uc, bool> getVector2uc(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a Vector2i
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a Vector2i and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a Lua object
         */
        std::pair<Vector2i, bool> getVector2i(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a Vector2ui
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a Vector2ui and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a Lua object
         */
        std::pair<Vector2ui, bool> getVector2ui(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a Vector2l
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a Vector2l and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a Lua object
         */
        std::pair<Vector2l, bool> getVector2l(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a Vector2ul
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a Vector2ul and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a Lua object
         */
        std::pair<Vector2ul, bool> getVector2ul(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a Vector2f
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a Vector2f and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a Lua object
         */
        std::pair<Vector2f, bool> getVector2f(const std::string &name) const;
        /**
         * \brief Gets the value of the variable "name" as a Vector2d
         * 
         * If the variable does not exist, the second value of the pair is false
         * \param name The name of the variable
         * \return A pair (value, found) with value a Vector2d and found a boolean (true iff the variable exists)
         * \throw engine::errors::WrongType when the value can not be converted to a Lua object
         */
        std::pair<Vector2d, bool> getVector2d(const std::string &name) const;

        void loadFromJSON(const nlohmann::json &json);
        nlohmann::json createJSON() const;

        /**
         * \brief Registers the Lua functions for the MemberStorage class
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

    protected:
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
         * \return The string to print
         */
        virtual inline std::string getLogErrorPrefix() const = 0;

        /**
         * \brief Registers a member variable to be accessed through get and set functions
         * 
         * The name of the variable can be arbitrary set (it does not have to be the same as the variable name in the C++ class).
         * 
         * If a free variable with the same name is already defined, the free variable is erased.
         * 
         * \param name The name under which to register the variable
         * \param member The pointer to the variable 
         * \see PositionComponent.cpp for an example
         */
        template <typename T>
        void registerMember(const std::string &name, T* member) {
            if (mapMembers.find(name) != mapMembers.end()) {
                m_context.logger.log(getLogErrorPrefix() + ": registerMember: " + name + " is already used. The assignment will be overwritten by the new assignation.", LogType::Warning);
            }

            mapFree.erase(name);
            mapMembers.insert(std::make_pair(name, member));
        }

    private:
        /**
         * \brief The internal container
         */
        using Container = std::map<std::string, std::any>;

    private:
        /**
         * \brief Sets the variable of the given name to the given value.
         * 
         * If the variable is a registered member, it also performs a type check.
         * If the check fails, a warning message is printed in the log and nothing changes in the variables.
         * 
         * If the variable is not a registered member, it uses a free variable.
         * 
         * \param name The name of the variable
         * \param value The new value
         * \param type The type of the variable to print in the log message
         */
        template <typename T>
        inline void setWithErrorMessage(const std::string &name, const T &value, const std::string &type) noexcept {
            if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
                if (itr->second.type() == typeid(T*)) {
                    convertAndSet<T>(itr->second, value);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with " + type, LogType::Warning);
                }
            }
            else {
                mapFree[name] = value;
            }
        }

        /**
         * \brief Gets the variable of the given name to the given value.
         * 
         * It performs a type check.
         * If the check fails, a warning message is printed in the log and nothing changes in the variables.
         * 
         * \param name The name of the variable
         * \param value The new value
         * \param type The type of the variable to print in the log message
         */
        template <typename T>
        inline std::pair<T, bool> getWithErrorMessage(const std::string &name, const T &defaultValue, const std::string &type) const {
            std::pair<T, bool> pair = std::make_pair(defaultValue, false);

            if (auto itr = getIterator(name); itr != mapMembers.end()) {
                if (itr->second.type() == typeid(T*)) {
                    pair = std::make_pair(getValue<T>(itr->second), true);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a " + type + " variable", LogType::Warning);
                }
            }
            else {
                logUnknownVariable(name);
            }

            return pair;
        }

        /**
         * \brief Writes the value of given type in a std::any variable
         * \param variable The variable to modify
         * \param value The value to write
         * \tparam T The type of the variable
         */
        template <typename T>
        inline void convertAndSet(std::any variable, T value) noexcept {
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
        inline T getValue(std::any variable) const {
            try {
                T *va = std::any_cast<T*>(variable);
                return *va;
            }
            catch (const std::bad_any_cast &e) {
                throw errors::WrongType(e.what());
            }
        }

        /**
         * \brief Puts a warning message in the log indicating that the given name is unknown.
         * \param name The name of the variable
         */
        inline void logUnknownVariable(const std::string &name) const;

        /**
         * \brief Gets the iterator pointing to the variable called "name", if it exists.
         * 
         * It looks in both members and free variables.
         * 
         * If "name" is unknown, a log message is written and the returned iterator is equal to mapMembers.end()
         * \param name The name of the variable
         * \return The iterator
         */
        inline Container::const_iterator getIterator(const std::string &name) const;

    private:
        /**
         * \brief The registered member variables
         */
        Container mapMembers;
        /**
         * \brief The free (i.e., non-member) variables
         */
        Container mapFree;

        Context &m_context;
    };
}