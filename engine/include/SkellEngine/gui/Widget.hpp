#pragma once

#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include "SkellEngine/math/Vector2.hpp"
#include "SkellEngine/files/FileManager.hpp"
#include "SkellEngine/states/StateContext.hpp"

namespace engine::gui {
    class Widget {
    public:
        using Ptr = std::shared_ptr<Widget>;

    public:
        Widget(const Widget&) = delete;
        virtual ~Widget() = default;

        virtual bool handleEvent(const sf::Event &event);

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;

        /**
         * \brief Gets the position of the widget.
         * \param relative If true and if the position is set to be relative, the relative position is returned. Otherwise, the absolute position is computed (if needed) and returned.
         * \return The position
         */
        Vector2d getPosition(bool relative = true) const noexcept;
        void setPosition(const Vector2d &position, bool relative = true) noexcept;

        double getRotation() const noexcept;
        void setRotation(double rotation) noexcept;

        bool getVisible() const noexcept;
        bool isVisible() const noexcept;
        void setVisible(bool visible) noexcept;

    protected:
        Widget(states::StateContext &context) noexcept;

        states::StateContext &getStateContext() noexcept;
        const states::StateContext &getStateContext() const noexcept;

    private:
        states::StateContext &m_context;
        Vector2d m_position;
        bool m_relativePosition;
        double m_rotation;
        bool m_visible;
    };
}