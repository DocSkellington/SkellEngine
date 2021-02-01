#pragma once

#include <string>
#include <list>

#include <SFML/Graphics/Text.hpp>
#include <unicode/unistr.h>

#include "SkellEngine/gui/Widget.hpp"
#include "SkellEngine/gui/TextStyle.hpp"
#include "SkellEngine/Color.hpp"

namespace engine::gui {
    /**
     * \todo Handle fonts
     */
    class Label : public Widget {
    public:
        using Ptr = std::shared_ptr<Label>;

    public:
        static Ptr create(states::StateContext &context);
        static Ptr create(states::StateContext &context, const icu::UnicodeString &text);

    public:
        Label(states::StateContext &context);
        ~Label() = default;

        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        void setText(const icu::UnicodeString &text);
        void setFont(const sf::Font &font);

        void setTextSize(uint32_t size);
        void setTextStyle(TextStyle style);
        TextStyle getTextStyle() const;

        void setTextColor(const Color &color);
        void setOutlineColor(const Color &color);

        void setSize(const Vector2d &size, bool relative = true);
        void setSize(double width, double height, bool relative = true);
        void setAutoSize(bool autoSize);

    private:
        void rearrangeText();

    private:
        Color m_textColor;
        Color m_outlineColor;
        sf::Font &m_font;
        uint32_t m_characterSize;
        TextStyle m_textStyle;
        bool m_autoSize;
        bool m_relativeSize;
        Vector2d m_boxSize;
        // std::list<icu::UnicodeString> m_words;
        icu::UnicodeString m_text;
        std::list<sf::Text> m_lines;
    };
}