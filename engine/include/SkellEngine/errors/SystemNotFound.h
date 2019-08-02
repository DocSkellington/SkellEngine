#pragma once

#include <stdexcept>
#include <string>

#include <SkellEngine/tmxlite/Log.hpp>

/**
 * \brief The errors specific to this engine
 */
namespace engine::errors {
    /**
    * \brief Defines the error to throw when a file does not exist
    */
    class SystemNotFound : public std::runtime_error {
    public:
        explicit SystemNotFound(const std::string &message) : runtime_error(message) { tmx::Logger::log(message, tmx::Logger::Type::Error); }
        explicit SystemNotFound(const char *message) : runtime_error(message) { tmx::Logger::log(message, tmx::Logger::Type::Error); }
    };
}