#pragma once

#include <stdexcept>
#include <string>

namespace engine::errors {
    /**
     * \brief The error to throw if a map is invalid (due to an invalid layer or something else)
     */
    class InvalidMap : public std::runtime_error {
    public:
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit InvalidMap(const std::string &message) : runtime_error(message) {  }
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit InvalidMap(const char *message) : runtime_error(message) {  }
    };
}