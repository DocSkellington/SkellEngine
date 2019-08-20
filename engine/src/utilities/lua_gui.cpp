#include "SkellEngine/utilities/lua_gui.h"

using namespace tgui;

// We disable the automatic to_string bindings for some types
namespace sol {
    template<>
    struct is_to_stringable<Gui> : std::false_type {};

    template<>
    struct is_to_stringable<AbsoluteOrRelativeValue> : std::false_type {};

    template<>
    struct is_to_stringable<Theme> : std::false_type {};

    template<>
    struct is_to_stringable<SignalWidgetBase> : std::false_type {};

    template<>
    struct is_to_stringable<Widget> : std::false_type {};

    template<>
    struct is_to_stringable<ClickableWidget> : std::false_type {};

    template<>
    struct is_to_stringable<Button> : std::false_type {};

    template<>
    struct is_to_stringable<GuiContainer> : std::false_type {};
}

namespace engine::utilities {
    void registerWidgets(sol::state &lua) {
        lua.new_usertype<Widget>("Widget",
            "setPosition", sol::overload(
                sol::resolve<void(const Layout2d&)>(&Widget::setPosition),
                sol::resolve<void(Layout, Layout)>(&Widget::setPosition),
                [](Widget::Ptr widget, int x, int y) { widget->setPosition(x, y); }
            )
        );

        lua.new_usertype<Button>("Button",
            "create", sol::factories(&Button::create, [](const std::string &string) { return Button::create(string); }),
            // "setSize", &Button::setSize,
            "setEnabled", &Button::setEnabled,
            "setText", &Button::setText,
            sol::base_classes, sol::bases<tgui::Widget>()
        );
    }

    void registerTGUILuaFunctions(sol::state &lua, std::shared_ptr<tgui::Gui> gui) {
        registerWidgets(lua);

        lua.new_usertype<AbsoluteOrRelativeValue>("AbsoluteOrRelativeValue",
            sol::constructors<AbsoluteOrRelativeValue(), AbsoluteOrRelativeValue(const std::string &)>(),
            "getValue", &AbsoluteOrRelativeValue::getValue,
            "getRatio", &AbsoluteOrRelativeValue::getValue,
            "isConstant", &AbsoluteOrRelativeValue::isConstant
        );

        lua.new_usertype<Theme>("Theme",
            sol::constructors<Theme(), Theme(const std::string&)>(),
            "load", &Theme::load,
            "getPrimary", &Theme::getPrimary,
            "setDefault", &Theme::setDefault,
            "getDefault", &Theme::getDefault
        );

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
            "add", sol::overload(
                &Gui::add,
                [](std::shared_ptr<Gui> gui, Button::Ptr widget) { gui->add(widget); },
                [](std::shared_ptr<Gui> gui, const Widget::Ptr& widget) { gui->add(widget); }
            ),
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