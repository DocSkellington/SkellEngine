#pragma once

#include <stdexcept>
#include <string>

namespace engine::errors {
    /**
    * \brief Defines the error to throw when a file does not exist
    */
    class StateNotFound : public std::runtime_error {
    public:
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit StateNotFound(const std::string &message) : runtime_error(message) { }
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit StateNotFound(const char *message) : runtime_error(message) { }
    };
}