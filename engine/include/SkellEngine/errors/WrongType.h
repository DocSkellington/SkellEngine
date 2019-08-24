#pragma once

#include <stdexcept>
#include <string>

#include "SkellEngine/tmxlite/Log.hpp"

namespace engine::errors {
    /**
     * \brief The error to throw if a variable in a MemberStorage instance can not be converted to the requested type
     */
    class WrongType : public std::runtime_error {
    public:
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit WrongType(const std::string &message) : runtime_error(message) { tmx::Logger::log(message, tmx::Logger::Type::Error); }
        /**
         * \brief Constructor
         * \param message The message to send with the error
         */
        explicit WrongType(const char *message) : runtime_error(message) { tmx::Logger::log(message, tmx::Logger::Type::Error); }
    };
}