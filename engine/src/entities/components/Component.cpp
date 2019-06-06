#include "SkellEngine/entities/components/Component.h"

#include "SkellEngine/entities/components/ExternComponent.h"
#include "SkellEngine/utilities/json_lua.h"
#include "SkellEngine/Context.h"

namespace engine::entities::components {
    Component::Component() {

    }

    Component::~Component() {

    }

    template <typename T>
    void convertAndSet(std::any variable, T value) noexcept {
        T *va = std::any_cast<T*>(variable);
        *va = value;
    }

    void Component::set(const std::string &name, int value) {
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
                tmx::Logger::log("Component: set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void Component::set(const std::string &name, long value) {
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
                tmx::Logger::log("Component: set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void Component::set(const std::string &name, float value) {
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
                tmx::Logger::log("Component: set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void Component::set(const std::string &name, double value) {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(double*)) {
                convertAndSet<double>(itr->second, value);
            }
            else {
                tmx::Logger::log("Component: set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void Component::set(const std::string &name, bool value) {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(bool*)) {
                convertAndSet<bool>(itr->second, value);
            }
            else {
                tmx::Logger::log("Component: set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void Component::set(const std::string &name, const char* value) {
        set(name, std::string(value));
    }

    void Component::set(const std::string &name, const std::string& value) {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(std::string*)) {
                convertAndSet<std::string>(itr->second, value);
            }
            else {
                tmx::Logger::log("Component: set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with std::string", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    void Component::set(const std::string &name, sol::nil_t) {
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
                tmx::Logger::log("Component: set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

    }

    void Component::set(const std::string &name, const sol::table& value) {
        set(name, utilities::lua_to_json(value));
    }

    void Component::set(const std::string &name, const nlohmann::json &value) {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(nlohmann::json*)) {
                convertAndSet<nlohmann::json>(itr->second, value);
            }
            else {
                tmx::Logger::log("Component: set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with std::string", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: set " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }
    }

    template <typename T>
    T getValue(std::any variable) {
        T *va = std::any_cast<T*>(variable);
        return *va;
    }

    std::pair<long, bool> Component::getInt(const std::string &name) {
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
                tmx::Logger::log("Component: get " + name + ": " + name + " is not an integer (int or long) variable", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: get " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }

    std::pair<double, bool> Component::getFloat(const std::string &name) {
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
                tmx::Logger::log("Component: get " + name + ": " + name + " is not a floating point (float or double) variable", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: get " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }
     
    std::pair<bool, bool> Component::getBool(const std::string &name) {
        std::pair<bool, bool> pair = std::make_pair(false, false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(bool*)) {
                pair = std::make_pair(getValue<bool>(itr->second), true);
            }
            else {
                tmx::Logger::log("Component: get " + name + ": " + name + " is not a boolean variable", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: get " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }
     
    std::pair<std::string, bool> Component::getString(const std::string &name) {
        std::pair<std::string, bool> pair = std::make_pair("", false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(std::string*)) {
                pair = std::make_pair(getValue<std::string>(itr->second), true);
            }
            else {
                tmx::Logger::log("Component: get " + name + ": " + name + " is not a std::string variable", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: get " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }

    std::pair<nlohmann::json, bool> Component::getJSON(const std::string &name) {
        std::pair<nlohmann::json, bool> pair = std::make_pair(nlohmann::json(), false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(nlohmann::json*)) {
                pair = std::make_pair(getValue<nlohmann::json>(itr->second), true);
            }
            else {
                tmx::Logger::log("Component: get " + name + ": " + name + " is not a JSON variable", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: get " + name + ": " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }
     
    std::pair<sol::object, bool> Component::getObject(const std::string &name) {
        std::pair<sol::object, bool> pair = std::make_pair(sol::nil_t(), false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(int*)) {
                pair = std::make_pair(sol::make_object<long>(*m_context->lua, getValue<int>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(long*)) {
                pair = std::make_pair(sol::make_object<long>(*m_context->lua, getValue<long>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(float*)) {
                pair = std::make_pair(sol::make_object<double>(*m_context->lua, getValue<float>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(double*)) {
                pair = std::make_pair(sol::make_object<double>(*m_context->lua, getValue<double>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(bool*)) {
                pair = std::make_pair(sol::make_object<bool>(*m_context->lua, getValue<bool>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(std::string*)) {
                pair = std::make_pair(sol::make_object<std::string>(*m_context->lua, getValue<std::string>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(nlohmann::json*)) {
                pair = std::make_pair(sol::make_object<nlohmann::json>(*m_context->lua, getValue<nlohmann::json>(itr->second)), true);
            }
            else {
                tmx::Logger::log("Component: getObject: " + name + " does not have an appropriate type", tmx::Logger::Type::Warning);
            }
        }
        else {
            tmx::Logger::log("Component: getObject: " + name + " is not a known member", tmx::Logger::Type::Warning);
        }

        return pair;
    }

    void Component::setContext(Context &context) {
        m_context = &context;
    }

    Component::Ptr Component::createInstance(Context &context, const std::string &componentType) {
        auto compoConstructor = getMapToComponent()->find(componentType);
        Component::Ptr ptr;
        if (compoConstructor == getMapToComponent()->end()) {
            ptr = std::make_shared<ExternComponent>();
        }
        else {
            ptr = compoConstructor->second();
        }
        ptr->setContext(context);
        return ptr;
    }

    void Component::luaFunctions(sol::state &lua) {
        lua.new_usertype<Component>("component",
            "get", &ExternComponent::getObject,

            "set", sol::overload(
                sol::resolve<void(const std::string&, long)>(&Component::set),
                sol::resolve<void(const std::string&, double)>(&Component::set),
                sol::resolve<void(const std::string&, bool)>(&Component::set),
                sol::resolve<void(const std::string&, const std::string&)>(&Component::set),
                sol::resolve<void(const std::string&, sol::nil_t)>(&Component::set),
                sol::resolve<void(const std::string&, const sol::table&)>(&Component::set)
            )
        );
    }

    Context &Component::getContext() {
        return *m_context;
    }

    std::shared_ptr<Component::MapType> Component::getMapToComponent() {
        static std::shared_ptr<Component::MapType> map = std::make_shared<Component::MapType>();
        return map;
    }
}