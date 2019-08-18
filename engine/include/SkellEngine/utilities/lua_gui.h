#pragma once

#include <sol/sol.hpp>

#include <TGUI/TGUI.hpp>

namespace engine::utilities {
    void registerTGUILuaFunctions(sol::state &lua, std::shared_ptr<tgui::gui> gui);
}