#pragma once

#include "SkellEngine/utilities/MemberStorage.h"

namespace engine::utilities {
    class ExternMemberStorage : public virtual MemberStorage {
    public:
        ExternMemberStorage(Context &context);

        void set(const std::string &name, int value) override;
        void set(const std::string &name, long value) override;
        void set(const std::string &name, float value) override;
        void set(const std::string &name, double value) override;
        void set(const std::string &name, bool value) override;
        void set(const std::string &name, const char* value) override;
        void set(const std::string &name, const std::string& value) override;
        void set(const std::string &name, const nlohmann::json& value) override;

        void set(const std::string &name, sol::nil_t value) override;
        void set(const std::string &name, const sol::table& value) override;

        bool has(const std::string &name) const noexcept override;

        std::pair<long, bool> getInt(const std::string &name) const override;
        std::pair<double, bool> getFloat(const std::string &name) const override;
        std::pair<bool, bool> getBool(const std::string &name) const override;
        std::pair<std::string, bool> getString(const std::string &name) const override;
        std::pair<nlohmann::json, bool> getJSON(const std::string &name) const override;
        std::pair<sol::object, bool> getObject(const std::string &name) const override;

        static void luaFunctions(sol::state &lua);

    protected:
        void setInternalJSONTable(const nlohmann::json &jsonTable);
        nlohmann::json &getInternalJSONTable();
        const nlohmann::json &getInternalJSONTable() const;

    private:
        nlohmann::json m_jsonTable;
    };
}