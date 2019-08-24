#pragma once

#include <sol/sol.hpp>

#include <TGUI/TGUI.hpp>

namespace engine::utilities {
    /**
     * \brief Register Lua functions for the GUI
     * \param lua The Lua state
     * \param gui The GUI object
     */
    void registerTGUILuaFunctions(sol::state &lua, std::shared_ptr<tgui::Gui> gui);
}