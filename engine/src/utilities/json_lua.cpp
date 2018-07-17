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
                        table.set(std::stoi(o.key()) + 1, o.value().get<double>());
                    else
                        table.set(o.key(), o.value().get<double>());
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

    bool simple_table(const sol::table &lua) {
        for (auto &itr : lua) {
            if (itr.first.get_type() != sol::type::number)
                return false;
        }
        return true;
    }

    nlohmann::json lua_to_json(const sol::table &lua) {
        nlohmann::json json;
        bool table = simple_table(lua);
        for (auto &itr : lua) {
            switch(itr.second.get_type()) {
            case sol::type::table:
                if (table) {
                    json[itr.first.as<int>()] = lua_to_json(itr.second);
                }
                else {
                    json[itr.first.as<std::string>()] = lua_to_json(itr.second);
                }
                break;
            case sol::type::number:
                if (table) {
                    json[itr.first.as<int>()] = itr.second.as<double>();
                }
                else {
                    json[itr.first.as<std::string>()] = itr.second.as<double>();
                }
                break;
            case sol::type::boolean:
                if (table) {
                    json[itr.first.as<int>()] = itr.second.as<bool>();
                }
                else {
                    json[itr.first.as<std::string>()] = itr.second.as<bool>();
                }
                break;
            case sol::type::string:
                if (table) {
                    json[itr.first.as<int>()] = itr.second.as<std::string>();
                }
                else {
                    json[itr.first.as<std::string>()] = itr.second.as<std::string>();
                }
                break;
            case sol::type::userdata:
                break;
            default:
                std::cout << itr.first.as<std::string>() << '\n';
                std::cout << "OTHER\n";
                break;
            }
        }

        return json;
    }
}