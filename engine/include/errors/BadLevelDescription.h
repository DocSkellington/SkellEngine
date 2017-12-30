#pragma once

#include <stdexcept>
#include <string>

namespace engine::errors {
    /**
     * \brief The error to throw if a level description is incorrect
     */
    class BadLevelDescription : public std::runtime_error {
    public:
        explicit BadLevelDescription(const std::string &message) : runtime_error(message) {}
        explicit BadLevelDescription(const char *message) : runtime_error(message) {}
    };
}