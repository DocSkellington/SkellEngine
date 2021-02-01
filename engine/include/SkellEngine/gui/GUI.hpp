#pragma once

#include <list>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "SkellEngine/gui/Widget.hpp"

namespace engine::gui {
    /**
     * Needs:
     *  - Must be able to handle keyboard, mouses, and controllers inputs
     *  - Must be possible to trigger a gui element using an event (for instance, automatically open a window every year in a simulation game)
     *  - Interactable GUI elements must trigger events
     */
    class GUI {
    public:
        GUI();

        bool add(Widget::Ptr widget);

        bool handleEvent(const sf::Event &event);

        void draw(sf::RenderTarget &target) const;

    private:
        std::list<Widget::Ptr> m_widgets;
    };
}