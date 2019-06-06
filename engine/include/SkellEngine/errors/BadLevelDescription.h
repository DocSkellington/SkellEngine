#pragma once

#include <stdexcept>
#include <string>

#include <SkellEngine/tmxlite/Log.hpp>

namespace engine::errors {
    /**
     * \brief The error to throw if a level description is incorrect
     * \ingroup Engine
     */
    class BadLevelDescription : public std::runtime_error {
    public:
        explicit BadLevelDescription(const std::string &message) : runtime_error(message) { tmx::Logger::log(message, tmx::Logger::Type::Error); }
        explicit BadLevelDescription(const char *message) : runtime_error(message) { tmx::Logger::log(message, tmx::Logger::Type::Error); }
    };
}