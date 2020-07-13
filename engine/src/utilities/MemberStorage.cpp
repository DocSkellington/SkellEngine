#include "SkellEngine/utilities/MemberStorage.hpp"

#include "SkellEngine/utilities/json_lua.hpp"
#include "SkellEngine/Context.hpp"

namespace engine::utilities {
    MemberStorage::MemberStorage(Context &context) :
        m_context(context) {

    }

    MemberStorage::~MemberStorage() {

    }

    void MemberStorage::set(const std::string &name, std::any value) noexcept {
        if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
            itr->second = value;
        }
        else {
            mapFree[name] = value;
        }
    }
    
    void MemberStorage::set(const std::string &name, char value) noexcept {
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
    
    void MemberStorage::set(const std::string &name, unsigned char value) noexcept {
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

    void MemberStorage::set(const std::string &name, int value) noexcept {
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

    void MemberStorage::set(const std::string &name, unsigned int value) noexcept {
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

    void MemberStorage::set(const std::string &name, long value) noexcept {
        if (auto itr = getIterator(name); itr != mapMembers.end()) {
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

    void MemberStorage::set(const std::string &name, unsigned long value) noexcept {
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

    void MemberStorage::set(const std::string &name, float value) noexcept {
        if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
            // We can store a float in: float, double
            if (itr->second.type() == typeid(double*)) {
                convertAndSet<double>(itr->second, value);
            }
            else if (itr->second.type() == typeid(float*)) {
                convertAndSet<float>(itr->second, value);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with float", LogType::Warning);
            }
        }
        else {
            mapFree[name] = value;
        }
    }

    void MemberStorage::set(const std::string &name, double value) noexcept {
        setWithErrorMessage(name, value, "double");
    }

    void MemberStorage::set(const std::string &name, bool value) noexcept {
        setWithErrorMessage(name, value, "bool");
    }

    void MemberStorage::set(const std::string &name, const char* value) noexcept {
        set(name, std::string(value));
    }

    void MemberStorage::set(const std::string &name, const std::string& value) noexcept {
        setWithErrorMessage(name, value, "std::string");
    }

    void MemberStorage::set(const std::string &name, sol::nil_t) noexcept {
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

    void MemberStorage::set(const std::string &name, const sol::table& value) noexcept {
        set(name, utilities::lua_to_json(value));
    }

    void MemberStorage::set(const std::string &name, const nlohmann::json &value) noexcept {
        setWithErrorMessage(name, value, "nlohmann::json");
    }

    void MemberStorage::set(const std::string &name, const Vector2c &value) noexcept {
        setWithErrorMessage(name, value, "Vector2c");
    }

    void MemberStorage::set(const std::string &name, const Vector2uc &value) noexcept {
        setWithErrorMessage(name, value, "Vector2uc");
    }

    void MemberStorage::set(const std::string &name, const Vector2i &value) noexcept {
        setWithErrorMessage(name, value, "Vector2i");
    }

    void MemberStorage::set(const std::string &name, const Vector2ui &value) noexcept {
        setWithErrorMessage(name, value, "Vector2ui");
    }

    void MemberStorage::set(const std::string &name, const Vector2l &value) noexcept {
        setWithErrorMessage(name, value, "Vector2l");
    }

    void MemberStorage::set(const std::string &name, const Vector2ul &value) noexcept {
        setWithErrorMessage(name, value, "Vector2ul");
    }

    void MemberStorage::set(const std::string &name, const Vector2f &value) noexcept {
        setWithErrorMessage(name, value, "Vector2f");
    }

    void MemberStorage::set(const std::string &name, const Vector2d &value) noexcept {
        setWithErrorMessage(name, value, "Vector2d");
    }

    bool MemberStorage::has(const std::string &name) const noexcept {
        if (auto itr = getIterator(name); itr != mapMembers.end()) {
            return itr->second.has_value();
        }
        return false;
    }

    std::pair<std::any, bool> MemberStorage::getAny(const std::string &name) const noexcept {
        std::pair<std::any, bool> pair = std::make_pair(std::any(), false);
        if (auto itr = getIterator(name); itr != mapMembers.end()) {
            pair = std::make_pair(*itr, true);
        }
        return pair;
    }

    std::pair<char, bool> MemberStorage::getChar(const std::string &name) const {
        return getWithErrorMessage<char>(name, 0, "char");
    }

    std::pair<unsigned char, bool> MemberStorage::getUnsignedChar(const std::string &name) const {
        return getWithErrorMessage<unsigned char>(name, 0, "unsigned char");
    }

    std::pair<int, bool> MemberStorage::getInt(const std::string &name) const {
        std::pair<int, bool> pair = std::make_pair(0, false);

        if (auto itr = getIterator(name); itr != mapMembers.end()) {
            if (itr->second.type() == typeid(char*)) {
                pair = std::make_pair(getValue<char>(itr->second), true);
            }
            else if (itr->second.type() == typeid(int*))  {
                pair = std::make_pair(getValue<int>(itr->second), true);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " can not be cast to an integer (i.e., it is not a char nor an int)", LogType::Warning);
            }
        }

        return pair;
    }

    std::pair<unsigned int, bool> MemberStorage::getUnsignedInt(const std::string &name) const {
        std::pair<unsigned int, bool> pair = std::make_pair(0, false);

        if (auto itr = getIterator(name); itr != mapMembers.end()) {
            if (itr->second.type() == typeid(unsigned char*)) {
                pair = std::make_pair(getValue<unsigned char>(itr->second), true);
            }
            else if (itr->second.type() == typeid(unsigned int*))  {
                pair = std::make_pair(getValue<unsigned int>(itr->second), true);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " can not be cast to an unsigned integer (i.e., it is not an unsigned char nor an unsigned int)", LogType::Warning);
            }
        }

        return pair;
    }

    std::pair<long, bool> MemberStorage::getLong(const std::string &name) const {
        std::pair<long, bool> pair = std::make_pair(0, false);

        if (auto itr = getIterator(name); itr != mapMembers.end()) {
            if (itr->second.type() == typeid(char*)) {
                pair = std::make_pair(getValue<char>(itr->second), true);
            }
            else if (itr->second.type() == typeid(int*))  {
                pair = std::make_pair(getValue<int>(itr->second), true);
            }
            else if (itr->second.type() == typeid(long*))  {
                pair = std::make_pair(getValue<long>(itr->second), true);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " can not be cast to a long", LogType::Warning);
            }
        }

        return pair;
    }

    std::pair<unsigned long, bool> MemberStorage::getUnsignedLong(const std::string &name) const {
        std::pair<unsigned long, bool> pair = std::make_pair(0, false);

        if (auto itr = getIterator(name); itr != mapMembers.end()) {
            if (itr->second.type() == typeid(unsigned char*)) {
                pair = std::make_pair(getValue<unsigned char>(itr->second), true);
            }
            else if (itr->second.type() == typeid(unsigned int*))  {
                pair = std::make_pair(getValue<unsigned int>(itr->second), true);
            }
            else if (itr->second.type() == typeid(unsigned long*))  {
                pair = std::make_pair(getValue<unsigned long>(itr->second), true);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " can not be cast to an unsigned long", LogType::Warning);
            }
        }

        return pair;
    }

    std::pair<float, bool> MemberStorage::getFloat(const std::string &name) const {
        return getWithErrorMessage<float>(name, 0.f, "float");
    }

    std::pair<double, bool> MemberStorage::getDouble(const std::string &name) const {
        std::pair<double, bool> pair = std::make_pair(0., false);

        if (auto itr = getIterator(name); itr != mapMembers.end()) {
            if (itr->second.type() == typeid(float*)) {
                pair = std::make_pair(getValue<float>(itr->second), true);
            }
            else if (itr->second.type() == typeid(double*))  {
                pair = std::make_pair(getValue<double>(itr->second), true);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a floating point (float or double) variable", LogType::Warning);
            }
        }

        return pair;
    }
     
    std::pair<bool, bool> MemberStorage::getBool(const std::string &name) const {
        return getWithErrorMessage(name, false, "boolean");
    }
     
    std::pair<std::string, bool> MemberStorage::getString(const std::string &name) const {
        return getWithErrorMessage<std::string>(name, "", "std::string");
    }

    std::pair<nlohmann::json, bool> MemberStorage::getJSON(const std::string &name) const {
        return getWithErrorMessage(name, nlohmann::json(), "JSON");
    }
     
    std::pair<sol::object, bool> MemberStorage::getObject(const std::string &name) const {
        std::pair<sol::object, bool> pair = std::make_pair(sol::nil_t(), false);

        if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
            if (itr->second.type() == typeid(char*)) {
                pair = std::make_pair(sol::make_object<char>(*getContext().lua, getValue<int>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(unsigned char*)) {
                pair = std::make_pair(sol::make_object<unsigned char>(*getContext().lua, getValue<int>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(int*)) {
                pair = std::make_pair(sol::make_object<int>(*getContext().lua, getValue<int>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(unsigned int*)) {
                pair = std::make_pair(sol::make_object<unsigned int>(*getContext().lua, getValue<int>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(long*)) {
                pair = std::make_pair(sol::make_object<long>(*getContext().lua, getValue<long>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(unsigned long*)) {
                pair = std::make_pair(sol::make_object<unsigned long>(*getContext().lua, getValue<long>(itr->second)), true);
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
            // TODO: add conversions to Vector2<T>
            else {
                m_context.logger.log(getLogErrorPrefix() + ": getObject: " + name + " does not have an appropriate type", LogType::Warning);
            }
        }
        else {
            logUnknownVariable(name);
        }

        return pair;
    }

    std::pair<Vector2c, bool> MemberStorage::getVector2c(const std::string &name) const {
        return getWithErrorMessage(name, Vector2c(), "Vector2c");
    }

    std::pair<Vector2uc, bool> MemberStorage::getVector2uc(const std::string &name) const {
        return getWithErrorMessage(name, Vector2uc(), "Vector2uc");
    }

    std::pair<Vector2i, bool> MemberStorage::getVector2i(const std::string &name) const {
        return getWithErrorMessage(name, Vector2i(), "Vector2i");
    }

    std::pair<Vector2ui, bool> MemberStorage::getVector2ui(const std::string &name) const {
        return getWithErrorMessage(name, Vector2ui(), "Vector2ui");
    }

    std::pair<Vector2l, bool> MemberStorage::getVector2l(const std::string &name) const {
        return getWithErrorMessage(name, Vector2l(), "Vector2l");
    }

    std::pair<Vector2ul, bool> MemberStorage::getVector2ul(const std::string &name) const {
        return getWithErrorMessage(name, Vector2ul(), "Vector2ul");
    }

    std::pair<Vector2f, bool> MemberStorage::getVector2f(const std::string &name) const {
        return getWithErrorMessage(name, Vector2f(), "Vector2f");
    }

    std::pair<Vector2d, bool> MemberStorage::getVector2d(const std::string &name) const {
        return getWithErrorMessage(name, Vector2d(), "Vector2d");
    }

    void MemberStorage::loadFromJSON(const nlohmann::json &json) {
        // TODO: iterate over each variable's name and put it in the storage
    }
    
    nlohmann::json MemberStorage::createJSON() const {
        // TODO: iterate over each registered variable and put it in a JSON
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
            ),

            "has", &MemberStorage::has
        );
    }

    void MemberStorage::logUnknownVariable(const std::string &name) const {
        m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known variable", LogType::Warning);
    }

    MemberStorage::Container::const_iterator MemberStorage::getIterator(const std::string &name) const {
        if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
            return itr;
        }
        else if (auto itr = mapFree.find(name); itr != mapFree.end()) {
            return itr;
        }
        else {
            logUnknownVariable(name);
            return mapMembers.end();
        }
    }

    Context &MemberStorage::getContext() {
        return m_context;
    }

    const Context &MemberStorage::getContext() const {
        return m_context;
    }
}