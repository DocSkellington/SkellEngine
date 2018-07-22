#pragma once

#include <json.hpp>
#include <sol.hpp>

namespace engine::utilities {
    /**
     * \brief Converts a JSON object into a Lua table.
     * 
     * The JSON must be correct. Otherwise, the behaviour is undefined. For example, it must not be empty.
     * 
     * Once converted, arrays start at 0.
     * 
     * \param json The JSON object
     * \param lua The Lua State (necessary to create the table(s))
     * \return The Lua table
     */
    sol::table json_to_lua(const nlohmann::json& json, sol::state& lua);

    /**
     * \brief Converts a Lua table into a JSON object
     * 
     * The Lua table must be correct. Otherwise, the behaviour is undefined.
     * 
     * Due to JSON limitations, if a Lua table mixes named and unnamed values, the JSON table indices are strings.
     * Due to Sol2 limitations, all numbers are stored as double, even if they were initially integers.
     * 
     * \param lua The Lua table
     * \return The JSON object corresponding to the Lua table
     */
    nlohmann::json lua_to_json(const sol::table &lua);
}