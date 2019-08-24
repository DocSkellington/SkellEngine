#pragma once

#include "SkellEngine/utilities/MemberStorage.h"

#include "SkellEngine/errors/WrongType.h"

namespace engine::utilities {
    /**
     * \brief Stores data in a JSON table
     * 
     * The interface is the same that for MemberStorage except that the variables can not be tied to an actual member of a class
     * \see MemberStorage for an actual storage for member variables
     */
    class ExternMemberStorage : public virtual MemberStorage {
    public:
        /**
         * \brief Constructor
         * \param context The context
         */
        ExternMemberStorage(Context &context);

        void set(const std::string &name, int value) noexcept override;
        void set(const std::string &name, long value) noexcept override;
        void set(const std::string &name, float value) noexcept override;
        void set(const std::string &name, double value) noexcept override;
        void set(const std::string &name, bool value) noexcept override;
        void set(const std::string &name, const char* value) noexcept override;
        void set(const std::string &name, const std::string& value) noexcept override;
        void set(const std::string &name, const nlohmann::json& value) noexcept override;

        void set(const std::string &name, sol::nil_t value) noexcept override;
        void set(const std::string &name, const sol::table& value) noexcept override;

        bool has(const std::string &name) const noexcept override;

        std::pair<long, bool> getInt(const std::string &name) const override;
        std::pair<double, bool> getFloat(const std::string &name) const override;
        std::pair<bool, bool> getBool(const std::string &name) const override;
        std::pair<std::string, bool> getString(const std::string &name) const override;
        std::pair<nlohmann::json, bool> getJSON(const std::string &name) const override;
        std::pair<sol::object, bool> getObject(const std::string &name) const override;

        /**
         * \brief Register Lua functions for ExternMemberStorage
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

    protected:
        /**
         * \brief Sets the internal JSON storage to the given JSON
         * \param jsonTable The new JSON storage
         */
        void setInternalJSONTable(const nlohmann::json &jsonTable);
        /** @{ */
        /**
         * \brief Gets the internal JSON storage
         * \return The internal JSON storage
         */
        nlohmann::json &getInternalJSONTable();
        const nlohmann::json &getInternalJSONTable() const;
        /** @} */

        /**
         * \brief Writes the value of given type in the internal JSON storage
         * \param name The name of the variable
         * \param value The value to write
         * \tparam T The type of the value
         */
        template <typename T>
        void setInternally(const std::string &name, T value) noexcept {
            m_jsonTable[name] = value;
        }

        /**
         * \brief Gets the value that is stored under the name 'name'
         * \param name The name of the variable
         * \param defaultValue The default value to put in the pair when the variable doesn't exist
         * \return A pair (the value, true iff the variable 'name' exists)
         * \throw errors::WrongType When the value can not be converted to T
         */
        template <typename T>
        std::pair<T, bool> getValue(const std::string &name, T defaultValue) const {
            if (auto itr = m_jsonTable.find(name) ; itr != m_jsonTable.end()) {
                try {
                    return std::make_pair(itr->get<T>(), true);
                }
                catch (const std::exception &e) {
                    throw errors::WrongType(getLogErrorPrefix() + ": get: impossible to convert a(n) " + itr->type_name() + " to " + typeid(T).name());
                }
            }
            else {
                return std::make_pair(defaultValue, false);
            }
        }

    private:
        nlohmann::json m_jsonTable;
    };
}