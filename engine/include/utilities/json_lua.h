#pragma once

#include <json.hpp>
#include <sol.hpp>

namespace engine::utilities {
    /**
     * \brief Converts a JSON object into a Lua table.
     * 
     * The JSON must be correct. Otherwise, the behaviour is undefined. For example, it must not be empty.
     * 
     * For an array, indices begin at 1, following the Lua grammar.
     * 
     * \param json The JSON object
     * \param lua The Lua State (necessary to create the table(s))
     * \return The Lua table
     */
    sol::table json_to_lua(const nlohmann::json& json, sol::state& lua);
}