#pragma once

#include <stdexcept>
#include <string>

/**
 * \brief The errors specific to this engine
 */
namespace engine::errors {
    /**
    * \brief Defines the error to throw when a file does not exist
    */
    class SystemNotFound : public std::runtime_error {
    public:
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit SystemNotFound(const std::string &message) : runtime_error(message) { }
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit SystemNotFound(const char *message) : runtime_error(message) { }
    };
}