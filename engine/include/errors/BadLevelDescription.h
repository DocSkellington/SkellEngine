#pragma once

#include <stdexcept>
#include <string>

#include "log/Logger.h"

namespace engine::errors {
    /**
     * \brief The error to throw if a level description is incorrect
     */
    class BadLevelDescription : public std::runtime_error {
    public:
        explicit BadLevelDescription(const std::string &message) : runtime_error(message) { log::log(message, log::LogLevel::Error); }
        explicit BadLevelDescription(const char *message) : runtime_error(message) { log::log(message, log::LogLevel::Error); }
    };
}