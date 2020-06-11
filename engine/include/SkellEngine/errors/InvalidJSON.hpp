#pragma once

#include <stdexcept>
#include <string>

namespace engine::errors {
    /**
     * \brief The error to throw if a JSON description is invalid
     */
    class InvalidJSON : public std::runtime_error {
    public:
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit InvalidJSON(const std::string &message) : runtime_error(message) {  }
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit InvalidJSON(const char *message) : runtime_error(message) {  }
    };
}