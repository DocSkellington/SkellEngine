#include "SkellEngine/gui/utilities/BreakString.hpp"

#include <memory>

#include <unicode/brkiter.h>

namespace engine::gui::utilities {
    bool isWhitespace(UChar character) {
        return u_isspace(character) || u_charType(character) == U_CONTROL_CHAR || u_charType(character) == U_NON_SPACING_MARK;
    }

    uint32_t wrapParagraphFixedGlyphSize(
        const icu::UnicodeString &string,
        const icu::Locale &locale,
        std::vector<Vector2i> &lineBounds,
        std::vector<int32_t> &trailingWhitespaces,
        int32_t maxCharactersPerLine,
        uint32_t maxNumberLines,
        icu::ErrorCode &errorCode
    ) {
        // We create a BreakIterator working on lines
        std::unique_ptr<icu::BreakIterator> breakItr {icu::BreakIterator::createLineInstance(locale, errorCode)};
        if (errorCode.isFailure()) {
            return 0;
        }
        breakItr->setText(string);

        // We can effectively search the line breaks
        uint32_t numLines = 0;
        int32_t startLine = 0;
        while (startLine < string.length() && numLines < maxNumberLines) {
            // We jump ahead in the text by the maximum number of characters
            int32_t endLine = startLine + maxCharactersPerLine;

            // If this puts us on a white space character, a control character (which includes newlines), or a non-spacing mark,
            // we continue reading characters until we find a character that can be displayed
            if (endLine < string.length()) {
                UChar character = string[endLine];
                while (endLine < string.length() && isWhitespace(character)) {
                    ++endLine;
                    character = string[endLine];
                }
            }

            // Then, we locate the last legal line-break decision at or before the current position
            // The "at or before" is what causes the "+ 1"
            endLine = breakItr->preceding(endLine + 1);

            if (startLine == endLine) {
                // If we wind back to where we started, there is no legal line break.
                // So, we simply break the line at the maximum number of characters
                lineBounds.emplace_back(startLine, startLine + maxCharactersPerLine);
                startLine += maxCharactersPerLine;
                trailingWhitespaces.push_back(0);
                numLines++;
            }
            else {
                // Otherwise, we got a good line break position.
                // We count the number of trailing whitespaces
                int32_t nextLineStart = endLine;
                for (endLine-- ; endLine > startLine && isWhitespace(string[endLine]) ; endLine--);

                lineBounds.emplace_back(startLine, endLine);
                trailingWhitespaces.push_back(nextLineStart - endLine - 1);
                startLine = nextLineStart;
                numLines++;
            }
        }
        return numLines;
    }

    uint32_t wrapParagraphVaryingGlyphSize(
        const icu::UnicodeString &string,
        const icu::Locale &locale,
        std::vector<Vector2i> &lineBounds,
        std::vector<int32_t> &trailingWhitespaces,
        Vector2d maxSize,
        sf::Font &font,
        unsigned int characterSize,
        bool bold,
        icu::ErrorCode &errorCode
    ) {
        // TODO: modify to take into account the size of each glyph
        // We create a BreakIterator working on lines
        std::unique_ptr<icu::BreakIterator> breakItr {icu::BreakIterator::createLineInstance(locale, errorCode)};
        if (errorCode.isFailure()) {
            return 0;
        }
        breakItr->setText(string);

        // We can effectively search the line breaks
        uint32_t numLines = 0;
        double height = 0;
        int32_t startLine = 0;
        while (startLine < string.length() && height < maxSize.y) {
            // First, we try to take as many characters as possible
            int32_t endLine = startLine;
            int32_t numberCharacters = 0;
            double width = 0;
            while (endLine < string.length() && width < maxSize.x) {
                UChar character = string[endLine];
                auto glyphSize = font.getGlyph(character, characterSize, bold).bounds;
                if (width + glyphSize.width > maxSize.x) {
                    break;
                }
                endLine++;
                numberCharacters++;
                width += glyphSize.width;
            }
            
            height += font.getGlyph(string[startLine], characterSize, bold).bounds.height;

            // If this puts us on a white space character, a control character (which includes newlines), or a non-spacing mark,
            // we continue reading characters until we find a character that can be displayed
            if (endLine < string.length()) {
                UChar character = string[endLine];
                while (endLine < string.length() && isWhitespace(character)) {
                    ++endLine;
                    character = string[endLine];
                }
            }

            // Then, we locate the last legal line-break decision at or before the current position
            // The "at or before" is what causes the "+ 1"
            endLine = breakItr->preceding(endLine + 1);

            if (startLine == endLine) {
                // If we wind back to where we started, there is no legal line break.
                // So, we simply break the line at the maximum number of characters
                lineBounds.emplace_back(startLine, startLine + numberCharacters);
                startLine += numberCharacters;
                trailingWhitespaces.push_back(0);
                numLines++;
            }
            else {
                // Otherwise, we got a good line break position.
                // We count the number of trailing whitespaces
                int32_t nextLineStart = endLine;
                for (endLine-- ; endLine > startLine && isWhitespace(string[endLine]) ; endLine--);

                lineBounds.emplace_back(startLine, endLine);
                trailingWhitespaces.push_back(nextLineStart - endLine - 1);
                startLine = nextLineStart;
                numLines++;
            }
        }
        return numLines;
    }
}