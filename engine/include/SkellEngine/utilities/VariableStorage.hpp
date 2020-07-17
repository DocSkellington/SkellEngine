#pragma once

#include <string>
#include <any>
#include <map>

#include <sol/sol.hpp>

#include <nlohmann/json.hpp>

#include "SkellEngine/errors/WrongType.hpp"
#include "SkellEngine/Context.hpp"
#include "SkellEngine/math/Vector2.hpp"
#include "SkellEngine/utilities/json_lua.hpp"

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
    class VariableStorage {
    public:
        /**
         * \brief The constructor
         * \param context A reference to the context of the engine
         */
        VariableStorage(Context &context);
        virtual ~VariableStorage();

        /**
         * \brief Sets a variable called name to the given value.
         * 
         * If "name" is not yet registered, a free variable is created.
         * Otherwise, the variable's value is overriden.
         * 
         * \param name The name of the variable
         * \param value The value
         */
        template <typename T>
        void set(const std::string &name, T value) noexcept {
            setWithErrorMessage(name, value, typeid(T).name());
        }

        template <>
        void set<std::any>(const std::string &name, std::any value) noexcept {
            if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
                itr->second = value;
            }
            else {
                mapFree[name] = value;
            }
        }
    
        template <>
        void set<char>(const std::string &name, char value) noexcept {
            if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
                // We can store a char in: char, int, long, float, double
                if (itr->second.type() == typeid(char*)) {
                    convertAndSet<char>(itr->second, value);
                }
                else if (itr->second.type() == typeid(int*)) {
                    convertAndSet<int>(itr->second, value);
                }
                else if (itr->second.type() == typeid(long*)) {
                    convertAndSet<long>(itr->second, value);
                }
                else if (itr->second.type() == typeid(float*)) {
                    convertAndSet<float>(itr->second, value);
                }
                else if (itr->second.type() == typeid(double*)) {
                    convertAndSet<double>(itr->second, value);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with char", LogType::Warning);
                }
            }
            else {
                mapFree[name] = value;
            }
        }

        template<>
        void set<unsigned char>(const std::string &name, unsigned char value) noexcept {
            if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
                // We can store an unsigned char in: unsigned char, unsigned int, unsigned long, float, double
                if (itr->second.type() == typeid(unsigned char*)) {
                    convertAndSet<unsigned char>(itr->second, value);
                }
                else if (itr->second.type() == typeid(unsigned int*)) {
                    convertAndSet<unsigned int>(itr->second, value);
                }
                else if (itr->second.type() == typeid(unsigned long*)) {
                    convertAndSet<unsigned long>(itr->second, value);
                }
                else if (itr->second.type() == typeid(float*)) {
                    convertAndSet<float>(itr->second, value);
                }
                else if (itr->second.type() == typeid(double*)) {
                    convertAndSet<double>(itr->second, value);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with unsigned char", LogType::Warning);
                }
            }
            else {
                mapFree[name] = value;
            }
        }

        template<>
        void set<int>(const std::string &name, int value) noexcept {
            if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
                // We can store a int in: int, long, float, double
                if (itr->second.type() == typeid(int*)) {
                    convertAndSet<int>(itr->second, value);
                }
                else if (itr->second.type() == typeid(long*)) {
                    convertAndSet<long>(itr->second, value);
                }
                else if (itr->second.type() == typeid(float*)) {
                    convertAndSet<float>(itr->second, value);
                }
                else if (itr->second.type() == typeid(double*)) {
                    convertAndSet<double>(itr->second, value);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with int", LogType::Warning);
                }
            }
            else {
                mapFree[name] = value;
            }
        }

        template <>
        void set<unsigned int>(const std::string &name, unsigned int value) noexcept {
            if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
                // We can store an unsigned int in: unsigned int, unsigned long, float, double
                if (itr->second.type() == typeid(unsigned int*)) {
                    convertAndSet<int>(itr->second, value);
                }
                else if (itr->second.type() == typeid(unsigned long*)) {
                    convertAndSet<long>(itr->second, value);
                }
                else if (itr->second.type() == typeid(float*)) {
                    convertAndSet<float>(itr->second, value);
                }
                else if (itr->second.type() == typeid(double*)) {
                    convertAndSet<double>(itr->second, value);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with unsigned int", LogType::Warning);
                }
            }
            else {
                mapFree[name] = value;
            }
        }

        template<>
        void set<long>(const std::string &name, long value) noexcept {
            if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
                // We can store a long in: long, double
                if (itr->second.type() == typeid(long*)) {
                    convertAndSet<long>(itr->second, value);
                }
                else if (itr->second.type() == typeid(double*)) {
                    convertAndSet<double>(itr->second, value);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with long", LogType::Warning);
                }
            }
            else {
                mapFree[name] = value;
            }
        }

        template <>
        void set<unsigned long>(const std::string &name, unsigned long value) noexcept {
            if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
                // We can store an unsigned long in: unsigned long, double
                if (itr->second.type() == typeid(unsigned long*)) {
                    convertAndSet<long>(itr->second, value);
                }
                else if (itr->second.type() == typeid(double*)) {
                    convertAndSet<double>(itr->second, value);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with unsigned long", LogType::Warning);
                }
            }
            else {
                mapFree[name] = value;
            }
        }

        template <>
        void set<float>(const std::string &name, float value) noexcept {
            if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
                // We can store a float in: float, double, long double
                if (itr->second.type() == typeid(float*)) {
                    convertAndSet<float>(itr->second, value);
                }
                else if (itr->second.type() == typeid(double*)) {
                    convertAndSet<double>(itr->second, value);
                }
                else if (itr->second.type() == typeid(long double*)) {
                    convertAndSet<long double>(itr->second, value);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with float", LogType::Warning);
                }
            }
            else {
                mapFree[name] = value;
            }
        }

        template <>
        void set<double>(const std::string &name, double value) noexcept {
            if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
                // We can store a double in: double, long double
                if (itr->second.type() == typeid(double*)) {
                    convertAndSet<double>(itr->second, value);
                }
                else if (itr->second.type() == typeid(long double*)) {
                    convertAndSet<long double>(itr->second, value);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with float", LogType::Warning);
                }
            }
            else {
                mapFree[name] = value;
            }
        }

        template <>
        void set<const char*>(const std::string &name, const char *str) noexcept {
            set(name, std::string(str));
        }

        template <>
        void set<sol::nil_t>(const std::string &name, sol::nil_t) noexcept {
            // We set the variable at the default value
            if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
                if (itr->second.type() == typeid(int*)) {
                    convertAndSet<int>(itr->second, 0);
                }
                else if (itr->second.type() == typeid(long*)) {
                    convertAndSet<long>(itr->second, 0L);
                }
                else if (itr->second.type() == typeid(float*)) {
                    convertAndSet<float>(itr->second, 0.f);
                }
                else if (itr->second.type() == typeid(double*)) {
                    convertAndSet<double>(itr->second, 0.);
                }
                else if (itr->second.type() == typeid(bool*)) {
                    convertAndSet<bool>(itr->second, false);
                }
                else if (itr->second.type() == typeid(std::string*)) {
                    convertAndSet<std::string>(itr->second, "");
                }
                else if (itr->second.type() == typeid(nlohmann::json*)) {
                    convertAndSet<nlohmann::json>(itr->second, nlohmann::json());
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with sol::nil_t", LogType::Warning);
                }
            }
            else {
                mapFree[name] = std::any();
            }
        }

        template<>
        void set<const sol::table&>(const std::string &name, const sol::table& value) noexcept {
            set(name, utilities::lua_to_json(value));
        }

        /**
         * \brief Checks if a variable called "name" exists
         * \param name The name of the variable
         * \return True iff the variable exists
         */
        bool has(const std::string &name) const noexcept;

        /**
         * \brief Gets the value of the variable called "name".
         * 
         * If returns a pair (T, bool) where the boolean is true if and only if there exists a variable called "name" and the variable's type could be cast to T.
         * If the boolean is false, the value of T is constructed from the default constructor.
         * 
         * \param name The name of the variable
         * \return A pair with the value and a boolean
         */
        template <typename T>
            requires std::is_default_constructible_v<T>
        std::pair<T, bool> get(const std::string &name) const {
            return get<T>(name, T());
        }

        /**
         * \brief Gets the value of the variable called "name" with an explicit default value if the "name" is not known.
         * 
         * If returns a pair (T, bool) where the boolean is true if and only if there exists a variable called "name" and the variable's type could be cast to T.
         * If the boolean is false, the value in the pair is defaultValue
         * 
         * \param name The name of the variable
         * \param defaultValue The default value
         * \return A pair with the value and a boolean
         */
        template <typename T>
        std::pair<T, bool> get(const std::string &name, const T &defaultValue) const {
            return getWithErrorMessage<T>(name, defaultValue, typeid(T).name());
        }

        template <>
        std::pair<std::any, bool> get<std::any>(const std::string &name, const std::any &defaultValue) const {
            std::pair<std::any, bool> pair = std::make_pair(defaultValue, false);
            if (auto itr = getIterator(name); itr != mapMembers.end()) {
                pair = std::make_pair(*itr, true);
            }
            return pair;
        }

        template <>
        std::pair<int, bool> get<int>(const std::string &name, const int &defaultValue) const {
            std::pair<int, bool> pair = std::make_pair(defaultValue, false);

            if (auto itr = getIterator(name); itr != mapMembers.end()) {
                if (isType<char>(itr->second)) {
                    pair = std::make_pair(getValue<char>(itr->second), true);
                }
                else if (isType<int>(itr->second)) {
                    pair = std::make_pair(getValue<int>(itr->second), true);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " can not be cast to an integer (i.e., it is not a char nor an int)", LogType::Warning);
                }
            }

            return pair;
        }

        template <>
        std::pair<unsigned int, bool> get<unsigned int>(const std::string &name, const unsigned int &defaultValue) const {
            std::pair<unsigned int, bool> pair = std::make_pair(defaultValue, false);

            if (auto itr = getIterator(name); itr != mapMembers.end()) {
                if (isType<unsigned char>(itr->second)) {
                    pair = std::make_pair(getValue<unsigned char>(itr->second), true);
                }
                else if (isType<unsigned int>(itr->second)) {
                    pair = std::make_pair(getValue<unsigned int>(itr->second), true);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " can not be cast to an unsigned integer (i.e., it is not an unsigned char nor an unsigned int)", LogType::Warning);
                }
            }

            return pair;
        }

        template<>
        std::pair<long, bool> get<long>(const std::string &name, const long &defaultValue) const {
            std::pair<long, bool> pair = std::make_pair(defaultValue, false);

            if (auto itr = getIterator(name); itr != mapMembers.end()) {
                if (isType<char>(itr->second)) {
                    pair = std::make_pair(getValue<char>(itr->second), true);
                }
                else if (isType<int>(itr->second)) {
                    pair = std::make_pair(getValue<int>(itr->second), true);
                }
                else if (isType<long>(itr->second)) {
                    pair = std::make_pair(getValue<long>(itr->second), true);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " can not be cast to a long", LogType::Warning);
                }
            }

            return pair;
        }

        template<>
        std::pair<unsigned long, bool> get<unsigned long>(const std::string &name, const unsigned long &defaultValue) const {
            std::pair<unsigned long, bool> pair = std::make_pair(defaultValue, false);

            if (auto itr = getIterator(name); itr != mapMembers.end()) {
                if (isType<unsigned char>(itr->second)) {
                    pair = std::make_pair(getValue<unsigned char>(itr->second), true);
                }
                else if (isType<unsigned int>(itr->second)) {
                    pair = std::make_pair(getValue<unsigned int>(itr->second), true);
                }
                else if (isType<unsigned long>(itr->second)) {
                    pair = std::make_pair(getValue<unsigned long>(itr->second), true);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " can not be cast to an unsigned long", LogType::Warning);
                }
            }

            return pair;
        }

        template <>
        std::pair<double, bool> get<double>(const std::string &name, const double &defaultValue) const {
            std::pair<double, bool> pair = std::make_pair(defaultValue, false);

            if (auto itr = getIterator(name); itr != mapMembers.end()) {
                if (isType<float>(itr->second)) {
                    pair = std::make_pair(getValue<float>(itr->second), true);
                }
                else if (isType<double>(itr->second)) {
                    pair = std::make_pair(getValue<double>(itr->second), true);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a floating point (float or double) variable", LogType::Warning);
                }
            }

            return pair;
        }

        template <>
        std::pair<long double, bool> get<long double>(const std::string &name, const long double &defaultValue) const {
            std::pair<long double, bool> pair = std::make_pair(defaultValue, false);

            if (auto itr = getIterator(name); itr != mapMembers.end()) {
                if (isType<float>(itr->second)) {
                    pair = std::make_pair(getValue<float>(itr->second), true);
                }
                else if (isType<double>(itr->second)) {
                    pair = std::make_pair(getValue<double>(itr->second), true);
                }
                else if (isType<long double>(itr->second)) {
                    pair = std::make_pair(getValue<long double>(itr->second), true);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a floating point (float or double) variable", LogType::Warning);
                }
            }

            return pair;
        }
     
        template <>
        std::pair<sol::object, bool> get<sol::object>(const std::string &name, const sol::object &defaultValue) const {
            std::pair<sol::object, bool> pair = std::make_pair(defaultValue, false);

            if (auto itr = getIterator(name); itr != mapMembers.end()) {
                if (isType<char>(itr->second)) {
                    pair = std::make_pair(sol::make_object<char>(*getContext().lua, getValue<int>(itr->second)), true);
                }
                else if (isType<unsigned char>(itr->second)) {
                    pair = std::make_pair(sol::make_object<unsigned char>(*getContext().lua, getValue<int>(itr->second)), true);
                }
                else if (isType<int>(itr->second)) {
                    pair = std::make_pair(sol::make_object<int>(*getContext().lua, getValue<int>(itr->second)), true);
                }
                else if (isType<unsigned int>(itr->second)) {
                    pair = std::make_pair(sol::make_object<unsigned int>(*getContext().lua, getValue<int>(itr->second)), true);
                }
                else if (isType<long>(itr->second)) {
                    pair = std::make_pair(sol::make_object<long>(*getContext().lua, getValue<long>(itr->second)), true);
                }
                else if (isType<unsigned long>(itr->second)) {
                    pair = std::make_pair(sol::make_object<unsigned long>(*getContext().lua, getValue<long>(itr->second)), true);
                }
                else if (isType<float>(itr->second)) {
                    pair = std::make_pair(sol::make_object<float>(*getContext().lua, getValue<float>(itr->second)), true);
                }
                else if (isType<double>(itr->second)) {
                    pair = std::make_pair(sol::make_object<double>(*getContext().lua, getValue<double>(itr->second)), true);
                }
                else if (isType<long double>(itr->second)) {
                    pair = std::make_pair(sol::make_object<long double>(*getContext().lua, getValue<double>(itr->second)), true);
                }
                else if (isType<bool>(itr->second)) {
                    pair = std::make_pair(sol::make_object<bool>(*getContext().lua, getValue<bool>(itr->second)), true);
                }
                else if (isType<std::string>(itr->second)) {
                    pair = std::make_pair(sol::make_object<std::string>(*getContext().lua, getValue<std::string>(itr->second)), true);
                }
                else if (isType<nlohmann::json>(itr->second)) {
                    pair = std::make_pair(sol::make_object<nlohmann::json>(*getContext().lua, getValue<nlohmann::json>(itr->second)), true);
                }
                // TODO: add conversions to Vector2<T>
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": get: the type of " + name + " (" + itr->second.type().name() + ") is not supported in Lua scripts", LogType::Warning);
                }
            }

            return pair;
        }

        void loadFromJSON(const nlohmann::json &json);

        /**
         * \brief Registers the Lua functions for the VariableStorage class
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
                if (isType<T>(itr->second)) {
                    pair = std::make_pair(getValue<T>(itr->second), true);
                }
                else {
                    m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a " + type + " variable (type of stored value: " + itr->second.type().name() + ")", LogType::Warning);
                }
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
         * \brief Gets the value of a std::any variable as the type T.
         * 
         * If the variable's type is a pointer, it is first dereferenced.
         * 
         * \param variable The variable to read
         * \return The value cast to T
         * \throw errors::WrongType when the value can not be converted to T
         * \tparam T The type of the variable
         */
        template <typename T>
        inline T getValue(std::any variable) const {
            try {
                if (variable.type() == typeid(T)) {
                    return std::any_cast<T>(variable);
                }
                else {
                    T *va = std::any_cast<T*>(variable);
                    return *va;
                }
            }
            catch (const std::bad_any_cast &e) {
                throw errors::WrongType(e.what());
            }
        }

        /**
         * \brief Checks whether the variable's actual type is T or T*
         * \param var The std::any variable
         */
        template <typename T>
        inline bool isType(const std::any &var) const {
            return var.type() == typeid(T) || var.type() == typeid(T*);
        }

        /**
         * \brief Puts a warning message in the log indicating that the given name is unknown.
         * \param name The name of the variable
         */
        void logUnknownVariable(const std::string &name) const;

        /**
         * \brief Gets the iterator pointing to the variable called "name", if it exists.
         * 
         * It looks in both members and free variables.
         * 
         * If "name" is unknown, a log message is written and the returned iterator is equal to mapMembers.end()
         * \param name The name of the variable
         * \return The iterator
         */
        Container::const_iterator getIterator(const std::string &name) const;

        /**
         * \brief A helper function for Lua scripts
         * \param name The name of the variable
         * \param defaultValue The default value
         * \return The pair
         * \see The get functions
         */
        std::pair<sol::object, bool> getForLua(const std::string &name, const sol::object &defaultValue) const;

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