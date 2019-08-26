#pragma once

#include <stdexcept>
#include <string>

namespace engine::errors {
    /**
     * \brief The error to throw if a feature (or a part of a feature) is not implemented
     */
    class NotImplemented : public std::runtime_error {
    public:
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit NotImplemented(const std::string &message) : runtime_error(message) {  }
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit NotImplemented(const char *message) : runtime_error(message) {  }
    };
}
