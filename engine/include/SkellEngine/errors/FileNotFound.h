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
     * \ingroup Engine
    */
    class FileNotFound : public std::runtime_error {
    public:
        explicit FileNotFound(const std::string &message) : runtime_error(message) { tmx::Logger::log(message, tmx::Logger::Type::Error); }
        explicit FileNotFound(const char *message) : runtime_error(message) { tmx::Logger::log(message, tmx::Logger::Type::Error); }
    };
}