#pragma once

#include <stdexcept>
#include <string>

#include "tmxlite/Log.hpp"

namespace engine::errors {
    /**
     * \brief The error to throw if a system/component is known but can not be constructed
     */
    class ConstructorNotValid : public std::runtime_error {
    public:
        explicit ConstructorNotValid(const std::string &message) : runtime_error(message) { tmx::Logger::log(message, tmx::Logger::Type::Error); }
        explicit ConstructorNotValid(const char *message) : runtime_error(message) { tmx::Logger::log(message, tmx::Logger::Type::Error); }
    };
}