#pragma once

#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <unicode/unistr.h>
#include <unicode/locid.h>
#include <unicode/errorcode.h>

#include "SkellEngine/math/Vector2.hpp"

namespace engine::gui::utilities {
    /**
     * \brief Wraps a paragraph (a text) such that each line has at most maxCharactersPerLine characters.
     * 
     * It returns the number of lines and writes the index of the first and last characters of each line in lineStarts.
     * 
     * The locale is used to determine when a sentence can be cut.
     * Inspired from http://userguide.icu-project.org/boundaryanalysis.
     * \param string The string to wrap. It is considered as a single paragraph, no matter how many empty lines there are between two sentences.
     * \param locale The locale to determine where we can break the lines.
     * \param lineStarts The vector in which to put the index of each starting line
     * \param trailingWhitespaces The number of trailing whitespaces each line
     * \param maxNumberLines The maximum number of lines
     * \param maxCharactersPerLine The maximum number of characters per line
     * \param errorCode If ICU encounters a problem, it writes into errorCode
     * \return The number of lines
     */
    uint32_t wrapParagraphFixedGlyphSize(
        const icu::UnicodeString &string,
        const icu::Locale &locale,
        std::vector<Vector2i> &lineBounds,
        std::vector<int32_t> &trailingWhitespaces,
        int32_t maxCharactersPerLine,
        uint32_t maxNumberLines,
        icu::ErrorCode &errorCode
    );

    uint32_t wrapParagraphVaryingGlyphSize(
        const icu::UnicodeString &string,
        const icu::Locale &locale,
        std::vector<Vector2i> &lineStarts,
        std::vector<int32_t> &trailingWhitespaces,
        Vector2d maxSize,
        sf::Font &font,
        unsigned int characterSize,
        bool bold,
        icu::ErrorCode &errorCode
    );
}