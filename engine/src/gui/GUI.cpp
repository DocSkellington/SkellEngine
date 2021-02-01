#include "SkellEngine/gui/GUI.hpp"

namespace engine::gui {
    GUI::GUI() {

    }

    bool GUI::add(Widget::Ptr widget) {
        m_widgets.push_back(widget);
        return true;
    }

    bool GUI::handleEvent(const sf::Event &event) {
        for (auto widget : m_widgets) {
            if (!widget->handleEvent(event)) {
                return false;
            }
        }
        return true;
    }

    void GUI::draw(sf::RenderTarget &target) const {
        for (auto widget : m_widgets) {
            if (widget->isVisible()) {
                sf::RenderStates states = sf::RenderStates::Default;
                widget->draw(target, states);
            }
        }
    }
}