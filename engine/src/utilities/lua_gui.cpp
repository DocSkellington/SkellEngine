#include "SkellEngine/utilities/lua_gui.h"

using namespace tgui;

namespace sol {
    template<>
    struct is_to_stringable<Gui> : std::false_type {};

    template<>
    struct is_to_stringable<GuiContainer> : std::false_type {};
}

namespace engine::utilities {
    void registerTGUILuaFunctions(sol::state &lua, std::shared_ptr<tgui::Gui> gui) {
        lua.new_usertype<Gui>("Gui",
            "setTarget", &Gui::setTarget,
            "getTarget", &Gui::getTarget,
            "setView", &Gui::setView,
            "getView", &Gui::getView,
            "handleEvent", &Gui::handleEvent,
            "setTabKeyUsageEnabled", &Gui::setTabKeyUsageEnabled,
            "isTabKeyUsageEnabled", &Gui::isTabKeyUsageEnabled,
            "draw", &Gui::draw,
            "getContainer", &Gui::getContainer,
            "setFont", &Gui::setFont,
            "getFont", &Gui::getFont,
            "getWidgets", &Gui::getWidgets,
            // Doesn't work:
            // "getWidgetNames", &Gui::getWidgetNames,
            "add", &Gui::add,
            "get", static_cast<Widget::Ptr (Gui::*)(const sf::String&) const>(&Gui::get),
            "remove", &Gui::remove,
            "removeAllWidgets", &Gui::removeAllWidgets,
            "setWidgetName", &Gui::setWidgetName,
            "getWidgetName", &Gui::getWidgetName,
            "focusNextWidget", &Gui::focusNextWidget,
            "focusPreviousWidget", &Gui::focusPreviousWidget,
            "unfocusAllWidgets", &Gui::unfocusAllWidgets,
            "uncheckRadioButtons", &Gui::uncheckRadioButtons,
            "setOpacity", &Gui::setOpacity,
            "getOpacity", &Gui::getOpacity,
            "loadWidgetsFromFile", &Gui::loadWidgetsFromFile,
            "saveWidgetsToFile", &Gui::saveWidgetsToFile
        );

        lua["game"]["gui"] = gui;
    }
}