#pragma once

#include <stdexcept>
#include <string>

namespace engine::errors {
    /**
     * \brief The error to throw if a level description is incorrect
     */
    class BadLevelDescription : public std::runtime_error {
    public:
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit BadLevelDescription(const std::string &message) : runtime_error(message) { }
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit BadLevelDescription(const char *message) : runtime_error(message) { }
    };
}