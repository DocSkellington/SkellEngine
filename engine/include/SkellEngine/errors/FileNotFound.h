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
    class FileNotFound : public std::runtime_error {
    public:
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit FileNotFound(const std::string &message) : runtime_error(message) { }
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit FileNotFound(const char *message) : runtime_error(message) { }
    };
}