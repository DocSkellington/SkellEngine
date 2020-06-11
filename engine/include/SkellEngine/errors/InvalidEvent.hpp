#pragma once

#include <stdexcept>
#include <string>

namespace engine::errors {
    /**
     * \brief The error to throw if a variable in an event does not contain the expected values
     */
    class InvalidEvent : public std::runtime_error {
    public:
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit InvalidEvent(const std::string &message) : runtime_error(message) {  }
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit InvalidEvent(const char *message) : runtime_error(message) {  }
    };
}