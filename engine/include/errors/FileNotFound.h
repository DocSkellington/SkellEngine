#pragma once

#include <stdexcept>
#include <string>
#include "log/Logger.h"

/**
 * \brief The errors specific to this engine
 */
namespace engine::errors {
    /**
    * \brief Defines the error to throw when a file does not exist
    */
    class FileNotFound : public std::runtime_error {
    public:
        explicit FileNotFound(const std::string &message) : runtime_error(message) { log::log(message, log::LogLevel::Error); }
        explicit FileNotFound(const char *message) : runtime_error(message) { log::log(message, log::LogLevel::Error); }
    };
}