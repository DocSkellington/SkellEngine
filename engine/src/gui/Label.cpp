#include "SkellEngine/gui/Label.hpp"

#include <string>
#include <iostream>

#include <unicode/brkiter.h>
#include <unicode/errorcode.h>
#include <unicode/ustream.h>

#include "SkellEngine/gui/utilities/BreakString.hpp"

sf::String toSFMLString(const icu::UnicodeString &string) {
    std::string text;
    string.toUTF8String(text);
    return text;
}

namespace engine::gui {
    Label::Ptr Label::create(states::StateContext &context) {
        return std::make_shared<Label>(context);
    }

    Label::Ptr Label::create(states::StateContext &context, const icu::UnicodeString &text) {
        auto ptr = std::make_shared<Label>(context);
        ptr->setText(text);
        return ptr;
    }

    Label::Label(states::StateContext &context) :
        Widget(context),
        m_font(context.context.fileManager->getDefaultFont()),
        m_autoSize(true),
        m_relativeSize(true) {

    }

    void Label::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        for (auto &line : m_lines) {
            target.draw(line, states);
        }
    }

    void Label::setText(const icu::UnicodeString &text) {
        m_text = text;
        
        rearrangeText();
    }

    void Label::setTextColor(const Color &color) {
        m_textColor = color;
        rearrangeText();
    }

    void Label::setOutlineColor(const Color &color) {
        m_outlineColor = color;
        rearrangeText();
    }

    void Label::setFont(const sf::Font &font) {
        m_font = font;
        rearrangeText();
    }

    void Label::setTextSize(uint32_t size) {
        m_characterSize = size;
        rearrangeText();
    }

    void Label::setTextStyle(TextStyle style) {
        m_textStyle = style;
        rearrangeText();
    }

    TextStyle Label::getTextStyle() const {
        return m_textStyle;
    }

    void Label::setSize(const Vector2d &size, bool relative) {
        m_boxSize = size;
        m_relativeSize = relative;
    }

    void Label::setSize(double width, double height, bool relative) {
        setSize(Vector2d{width, height}, relative);
    }

    void Label::setAutoSize(bool autoSize) {
        m_autoSize = autoSize;
    }

    void Label::rearrangeText() {
        m_lines.clear();

        if (m_autoSize) {
            // We create a single line with the whole text
            sf::Text text;
            text.setString(toSFMLString(m_text));
            text.setFont(m_font);
            text.setPosition(getPosition());
            text.setStyle(static_cast<unsigned int>(m_textStyle));
            text.setFillColor(m_textColor);
            text.setCharacterSize(m_characterSize);
            text.setRotation(getRotation());
            m_lines.push_back(text);
        }
        else {
            Vector2d size = m_boxSize;
            if (m_relativeSize) {
                Vector2ui windowSize {getStateContext().context.window->getSize().x, getStateContext().context.window->getSize().y};
                size.x *= windowSize.x;
                size.y *= windowSize.y;
            }
            // First, we compute the maximum number of characters per line
            const uint32_t maxCharactersPerLine = m_boxSize.x / m_font.getGlyph(sf::String(L"a").getData()[0], m_characterSize, false).bounds.width;
            std::cout << "Maximum characters: " << maxCharactersPerLine << "\n";
            std::cout << m_boxSize.x << " " << m_characterSize << "\n";
            // And the maximum number of lines
            const uint32_t maxNumberLines = m_boxSize.y / m_characterSize;
            // Then, we call our helper function that returns the number of lines and writes the starting index of each line in a vector
            icu::ErrorCode errorCode;
            std::vector<Vector2i> lineBounds;
            std::vector<int32_t> trailingWhitespaces;

            bool isBold = (m_textStyle & TextStyle::Bold) == TextStyle::Bold;
            const uint32_t numLines = utilities::wrapParagraphVaryingGlyphSize(m_text, getStateContext().context.locale, lineBounds, trailingWhitespaces, m_boxSize, m_font, m_characterSize, isBold, errorCode);
            if (errorCode.isFailure()) {
                getStateContext().context.logger.log("Error while trying to break a text into lines: " + std::string(errorCode.errorName()) + ". The label's text will not be displayed.", engine::LogType::Error);
                return;
            }

            std::cout << m_text << " " << numLines << "\n";
            int32_t start = 0;
            for (std::size_t i = 0 ; i < lineBounds.size() ; i++) {
                std::cout << lineBounds.at(i) << " " << trailingWhitespaces.at(i) << " " << m_text.tempSubStringBetween(lineBounds[i].x, lineBounds[i].y) << "\n";
            }
        }
    }
}