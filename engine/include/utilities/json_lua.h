#pragma once

#include <json.hpp>
#include <sol.hpp>

namespace engine::utilities {
    void json_to_lua(const nlohmann::json& json, sol::table& lua) {
        if (json.is_array()) {
            for (std::size_t i = 0 ; i < json.size() ; i++) {
                lua[i] = json[i];
            }
        }
        else if (json.is_object()) {
            for (auto &o : json.items()) {
                if (o.value().is_object() || o.value().is_array()) {
                    lua.set(o.key(), json_to_lua(o.value()));
                }
                else if (o.value().is_boolean()) {
                    lua.set(o.key(), o.value().get<bool>());
                }
                else if (o.value().is_number_float()) {
                    lua.set(o.key(), o.value().get<float>());
                }
                else if (o.value().is_number_integer()) {
                    lua.set(o.key(), o.value().get<long>());
                }
                else if (o.value().is_number_unsigned()) {
                    lua.set(o.key(), o.value().get<unsigned long>());
                }
                else if (o.value().is_string()) {
                    lua[o.key()] = o.value().get<std::string>();
                }
            }
        }
        else {
            std::cout << "Hello\n";
        }
    }
}