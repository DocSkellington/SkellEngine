#include "SkellEngine/gui/Widget.hpp"

namespace engine::gui {
    bool Widget::handleEvent([[maybe_unused]] const sf::Event &event) {
        // By default, we do nothing
        return true;
    }

    Vector2d Widget::getPosition(bool relative) const noexcept {
        Vector2d position = m_position;
        if (relative) {
            position.x *= getStateContext().context.window->getSize().x;
            position.y *= getStateContext().context.window->getSize().y;
        }
        return m_position;
    }

    void Widget::setPosition(const Vector2d &position, bool relative) noexcept {
        m_position = position;
        m_relativePosition = relative;
    }

    double Widget::getRotation() const noexcept {
        return m_rotation;
    }

    void Widget::setRotation(double rotation) noexcept {
        m_rotation = rotation;
    }

    bool Widget::getVisible() const noexcept {
        return isVisible();
    }

    bool Widget::isVisible() const noexcept {
        return m_visible;
    }

    void Widget::setVisible(bool visible) noexcept {
        m_visible = visible;
    }

    Widget::Widget(states::StateContext &context) noexcept :
        m_context(context),
        m_position(0, 0),
        m_rotation(0),
        m_visible(true) {
        
    }

    states::StateContext &Widget::getStateContext() noexcept {
        return m_context;
    }

    const states::StateContext &Widget::getStateContext() const noexcept {
        return m_context;
    }
}