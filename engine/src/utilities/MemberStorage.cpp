#include "SkellEngine/utilities/MemberStorage.h"

#include "SkellEngine/utilities/json_lua.h"

#include "SkellEngine/Context.h"

namespace engine::utilities {
    MemberStorage::MemberStorage() {

    }

    MemberStorage::~MemberStorage() {

    }

    /**
     * \brief Writes the value of given type in a std::any variable
     * \param variable The variable to modify
     * \param value The value to write
     */
    template <typename T>
    void convertAndSet(std::any variable, T value) noexcept {
        T *va = std::any_cast<T*>(variable);
        *va = value;
    }

    void MemberStorage::set(const std::string &name, int value) {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
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
                tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, long value) {
        tmx::Logger::log("LONG");
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            // We can store a long in: long, double
            if (itr->second.type() == typeid(long*)) {
                convertAndSet<long>(itr->second, value);
            }
            else if (itr->second.type() == typeid(double*)) {
                convertAndSet<double>(itr->second, value);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, float value) {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            // We can store a float in: float, double
            if (itr->second.type() == typeid(double*)) {
                convertAndSet<double>(itr->second, value);
            }
            else if (itr->second.type() == typeid(float*)) {
                convertAndSet<float>(itr->second, value);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, double value) {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(double*)) {
                convertAndSet<double>(itr->second, value);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, bool value) {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(bool*)) {
                convertAndSet<bool>(itr->second, value);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, const char* value) {
        set(name, std::string(value));
    }

    void MemberStorage::set(const std::string &name, const std::string& value) {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(std::string*)) {
                convertAndSet<std::string>(itr->second, value);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with std::string", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, sol::nil_t) {
        // We set the variable at the default value
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
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
                tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

    }

    void MemberStorage::set(const std::string &name, const sol::table& value) {
        set(name, utilities::lua_to_json(value));
    }

    void MemberStorage::set(const std::string &name, const nlohmann::json &value) {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(nlohmann::json*)) {
                convertAndSet<nlohmann::json>(itr->second, value);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with std::string", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    /**
     * \brief Gets the value of a std::any variable as the type T
     * \param variable The variable to read
     * \return The value cast to T
     * \throw std::bad_any_cast When the value can not be converted to T
     */
    template <typename T>
    T getValue(std::any variable) {
        T *va = std::any_cast<T*>(variable);
        return *va;
    }

    std::pair<long, bool> MemberStorage::getInt(const std::string &name) const {
        std::pair<long, bool> pair = std::make_pair(0, false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(int*)) {
                pair = std::make_pair(getValue<int>(itr->second), true);
            }
            else if (itr->second.type() == typeid(long*))  {
                pair = std::make_pair(getValue<long>(itr->second), true);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not an integer (int or long) variable", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }

    std::pair<double, bool> MemberStorage::getFloat(const std::string &name) const {
        std::pair<double, bool> pair = std::make_pair(0., false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(float*)) {
                pair = std::make_pair(getValue<float>(itr->second), true);
            }
            else if (itr->second.type() == typeid(double*))  {
                pair = std::make_pair(getValue<double>(itr->second), true);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a floating point (float or double) variable", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }
     
    std::pair<bool, bool> MemberStorage::getBool(const std::string &name) const {
        std::pair<bool, bool> pair = std::make_pair(false, false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(bool*)) {
                pair = std::make_pair(getValue<bool>(itr->second), true);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a boolean variable", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }
     
    std::pair<std::string, bool> MemberStorage::getString(const std::string &name) const {
        std::pair<std::string, bool> pair = std::make_pair("", false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(std::string*)) {
                pair = std::make_pair(getValue<std::string>(itr->second), true);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a std::string variable", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }

    std::pair<nlohmann::json, bool> MemberStorage::getJSON(const std::string &name) const {
        std::pair<nlohmann::json, bool> pair = std::make_pair(nlohmann::json(), false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(nlohmann::json*)) {
                pair = std::make_pair(getValue<nlohmann::json>(itr->second), true);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a JSON variable", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }
     
    std::pair<sol::object, bool> MemberStorage::getObject(const std::string &name) const {
        std::pair<sol::object, bool> pair = std::make_pair(sol::nil_t(), false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(int*)) {
                pair = std::make_pair(sol::make_object<long>(*getContext().lua, getValue<int>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(long*)) {
                pair = std::make_pair(sol::make_object<long>(*getContext().lua, getValue<long>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(float*)) {
                pair = std::make_pair(sol::make_object<double>(*getContext().lua, getValue<float>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(double*)) {
                pair = std::make_pair(sol::make_object<double>(*getContext().lua, getValue<double>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(bool*)) {
                pair = std::make_pair(sol::make_object<bool>(*getContext().lua, getValue<bool>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(std::string*)) {
                pair = std::make_pair(sol::make_object<std::string>(*getContext().lua, getValue<std::string>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(nlohmann::json*)) {
                pair = std::make_pair(sol::make_object<nlohmann::json>(*getContext().lua, getValue<nlohmann::json>(itr->second)), true);
            }
            else {
                tmx::Logger::log(getLogErrorPrefix() + ": getObject: " + name + " does not have an appropriate type", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log(getLogErrorPrefix() + ": getObject: " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }

    void MemberStorage::luaFunctions(sol::state &lua) {
        lua.new_usertype<MemberStorage>("memberStorage",
            "get", &MemberStorage::getObject,

            "set", sol::overload(
                sol::resolve<void(const std::string&, double)>(&MemberStorage::set),
                sol::resolve<void(const std::string&, long)>(&MemberStorage::set),
                sol::resolve<void(const std::string&, bool)>(&MemberStorage::set),
                sol::resolve<void(const std::string&, const std::string&)>(&MemberStorage::set),
                sol::resolve<void(const std::string&, sol::nil_t)>(&MemberStorage::set),
                sol::resolve<void(const std::string&, const sol::table&)>(&MemberStorage::set)
            )
        );
    }

    void MemberStorage::setContext(Context &context) {
        m_context = &context;
    }

    Context &MemberStorage::getContext() {
        return *m_context;
    }

    const Context &MemberStorage::getContext() const {
        return *m_context;
    }
}