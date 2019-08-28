#include "SkellEngine/utilities/lua_gui.h"

using namespace tgui;

#define WidgetFunctions(T)                                                          \
    "setPosition", sol::overload(                                                   \
        sol::resolve<void(const Layout2d&)>(&T::setPosition),                       \
        sol::resolve<void(Layout, Layout)>(&T::setPosition),                        \
        [](T::Ptr widget, float x, float y) { widget->setPosition(x, y); }          \
    ),                                                                              \
    "getPosition", &T::getPosition,                                                 \
    "setSize", sol::overload(                                                       \
        sol::resolve<void(const Layout2d&)>(&T::setSize),                           \
        sol::resolve<void(Layout, Layout)>(&T::setSize),                            \
        [](T::Ptr widget, float x, float y) { widget->setSize(x, y); }              \
    ),                                                                              \
    "getSize", &T::getSize,                                                         \
    "getFullSize", &T::getFullSize,                                                 \
    "getAbsolutePosition", &T::getAbsolutePosition,                                 \
    "getWidgetOffset", &T::getWidgetOffset,                                         \
    "showWithEffect", &T::showWithEffect,                                           \
    "hideWithEffect", &T::hideWithEffect,                                           \
    "setVisible", &T::setVisible,                                                   \
    "isVisible", &T::isVisible,                                                     \
    "setEnabled", &T::setEnabled,                                                   \
    "isEnabled", &T::isEnabled,                                                     \
    "setFocused", &T::setFocused,                                                   \
    "isFocused", &T::isFocused,                                                     \
    "getWidgetType", &T::getWidgetType,                                             \
    "getParent", &T::getParent,                                                     \
    "moveToFront", &T::moveToFront,                                                 \
    "moveToBack", &T::moveToBack,                                                   \
    "setInheritedFont", &T::setInheritedFont,                                       \
    "getInheritedFont", &T::getInheritedFont,                                       \
    "setInheritedOpacity", &T::setInheritedOpacity,                                 \
    "getInheritedOpacity", &T::getInheritedOpacity,                                 \
    "setToolTip", &T::setToolTip,                                                   \
    "getToolTip", &T::getToolTip,                                                   \
    "canGainFocus", &T::canGainFocus,                                               \
    "isContainer", &T::isContainer,                                                 \
    "mouseOnWidget", &T::mouseOnWidget,                                             \
    "draw", &T::draw

// We disable the automatic to_string bindings for some types
namespace sol {
    template<>
    struct is_to_stringable<Gui> : std::false_type {};

    template<>
    struct is_to_stringable<AbsoluteOrRelativeValue> : std::false_type {};

    template<>
    struct is_to_stringable<Vector2f> : std::false_type {};

    template<>
    struct is_to_stringable<Theme> : std::false_type {};

    template<>
    struct is_to_stringable<Button> : std::false_type {};

    template<>
    struct is_to_stringable<GuiContainer> : std::false_type {};
}

namespace engine::utilities {
    void registerWidgets(sol::state &lua) {
        lua.new_usertype<Button>("Button",
            "create", sol::factories(&Button::create, [](const std::string &string) { return Button::create(string); }),
            "setEnabled", &Button::setEnabled,
            "setText", &Button::setText,
            WidgetFunctions(Button)
        );
    }

    void registerTGUILuaFunctions(sol::state &lua, std::shared_ptr<tgui::Gui> gui) {
        lua.new_usertype<Vector2f>("Vector2f",
            sol::constructors<Vector2f(), Vector2f(float x, float y), Vector2f(const sf::Vector2f&), Vector2f(const char*), Vector2f(std::string)>(),
            "x", &Vector2f::x,
            "y", &Vector2f::y
        );

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