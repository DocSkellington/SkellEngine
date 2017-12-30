#pragma once

#include <stdexcept>
#include <string>

namespace engine::errors {
    class BadLevelDescription : public std::runtime_error {
    public:
        explicit BadLevelDescription(const std::string &message) : runtime_error(message) {}
        explicit BadLevelDescription(const char *message) : runtime_error(message) {}
    };
}