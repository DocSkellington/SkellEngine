#include "utilities/json_lua.h"

namespace engine::utilities {
    // 'array' indicates if the current json object parsed is an array. If yes, the keys are converted into int. If no, the keys are still strings.
    sol::table json_to_lua(const nlohmann::json& json, sol::state& lua, bool array) {
        if (json.is_array() && !array) {
            return json_to_lua(json, lua, true);
        }
        else {
            sol::table table = lua.create_table();
            for (auto &o : json.items()) {
                if (o.value().is_object() || o.value().is_array()) {
                    if (array)
                        table.set(std::stoi(o.key()) + 1, json_to_lua(o.value(), lua, o.value().is_array()), o.value().is_array());
                    else
                        table.set(o.key(), json_to_lua(o.value(), lua, o.value().is_array()), o.value().is_array());
                }
                else if (o.value().is_boolean()) {
                    if (array)
                        table.set(std::stoi(o.key()) + 1, o.value().get<bool>());
                    else
                        table.set(o.key(), o.value().get<bool>());
                }
                else if (o.value().is_number_float()) {
                    if (array)
                        table.set(std::stoi(o.key()) + 1, o.value().get<float>());
                    else
                        table.set(o.key(), o.value().get<float>());
                }
                else if (o.value().is_number_unsigned()) {
                    if (array)
                        table.set(std::stoi(o.key()) + 1, o.value().get<unsigned long>());
                    else
                        table.set(o.key(), o.value().get<unsigned long>());
                }
                else if (o.value().is_number_integer()) {
                    if (array)
                        table.set(std::stoi(o.key()) + 1, o.value().get<long>());
                    else
                        table.set(o.key(), o.value().get<long>());
                }
                else if (o.value().is_string()) {
                    if (array)
                        table.set(std::stoi(o.key()) + 1, o.value().get<std::string>());
                    else
                        table.set(o.key(), o.value().get<std::string>());
                }
            }
            return table;
        }
    }

    sol::table json_to_lua(const nlohmann::json& json, sol::state& lua) {
        return json_to_lua(json, lua, false);
    }
}