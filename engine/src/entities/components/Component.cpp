#include "entities/components/Component.h"

#include "entities/components/ExternComponent.h"

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
                sol::resolve<void(const std::string&, const std::string&)>(&Component::set)
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