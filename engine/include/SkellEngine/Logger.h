#pragma once

#include <string>
#include <filesystem>
#include <fstream>

namespace engine {
    /**
     * \brief Where to print the log messages
     * 
     * Use None to disable the logger
     */
    enum class LogOutput {
        None,
        Console,
        File,
        All
    };

    /**
     * \brief The type (level) of the log message
     */
    enum class LogType {
        Info,
        Warning,
        Error
    };

    /**
     * \brief The logger
     */
    class Logger {
    public:
        /**
         * \brief Constructor
         */
        Logger() noexcept;
        Logger(const Logger&) = delete;

        /**
         * \brief Sets the minimum level a log message must have to be printed
         * 
         * For example, if the level is set to Warning, only Warning and Error messages are printed. The Info messages are ignored.
         */
        void setLogLevel(LogType level) noexcept;
        /**
         * \brief Sets the output file to print the log messages in
         * 
         * The logger will write in this file only if the output is set to File or All
         * \param path The path to the file
         */
        void setOutputFile(const std::filesystem::path &path) noexcept;
        /**
         * \brief Sets the output of the logger
         */
        void setLogOutput(LogOutput output) noexcept;

        /**
         * \brief Logs a message with the given type (level)
         * \param message The message
         * \param type The level of the message
         */
        void log(const std::string &message, LogType type = LogType::Info) const noexcept;
        /**
         * \brief Logs an exception message
         * 
         * It is equivalent to
         * \code{.cpp}
         * log(exception.what(), LogType::Error);
         * \endcode
         * \param exception The exception
         */
        void logError(const std::exception &exception) const noexcept;
        /**
         * \brief Logs an exception message prefixed by the given message
         * 
         * It is equivalent to
         * \code{.cpp}
         * log(message + "\n" + exception.what(), LogType::Error);
         * \endcode
         * \param message
         * \param exception The exception
         */
        void logError(const std::string &message, const std::exception &exception) const noexcept;

        /**
         * \brief Forces a message to be printed in the console
         * 
         * Log messages are always printed as Error messages by using this function
         * 
         * Please use the other log functions instead of this one if you have an access to the logger
         * \param message The message to print
         */
        static void forceLog(const std::string &message) noexcept;

    private:
        LogOutput m_output;
        LogType m_level;
        mutable std::ofstream m_outputFile;
    };
}