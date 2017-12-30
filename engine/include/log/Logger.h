#pragma once

#include <fstream>
#include <string>

/**
 * \brief The log system.
 * 
 * There are three independent log levels: Info, Warning and Error.
 */
namespace engine::log {
    /**
     * \brief The available log levels
     */
    enum LogLevel {
        /** No log is produced */
        LevelNone        = 0,
        /** Authorize the logs of type "Info" to be printed */
        Info        = 1 << 0,
        /** Authorize the logs of type "Warning" to be printed */
        Warning     = 1 << 1,
        /** Authorize the logs of type "Error" to be printed */
        Error       = 1 << 2,
        /** Authorize all log messages to be printed */
        LevelAll    = Info|Warning|Error
    };

    /**
     * \brief Where the logger must write its output.
     */ 
    enum LogOutput {
        /** No log is produced */
        OutputNone        = 0,
        /** The log messages will be printed in the console */
        Console     = 1 << 0,
        /** The log messages will be printed in the "log.txt" file (found in media/) */
        File        = 1 << 1,
        /** The log messages will be printed in the console and in the "log.txt" file */
        OutputAll   = Console | File
    };

    /**
     * \brief Defines the log messages that can be printed.
     * \param level The level flag; see engine::log::LogLevel
     */
    void setLogLevel(int l);
    /**
     * \brief Defines where the log messages can be printed.
     * \param output The output flag; see engine::log::LogOutput
     */
    void setLogOutput(int o);

    /**
     * \brief Initialises the logger.
     * 
     * It must be called before the first log call!
     * \param level The level flag; see engine::log::LogLevel
     * \param output The output flag; see engine::log::LogOutput
     */
    void startLogger(int l, int o);

    /**
     * \brief Closes the logger.
     */
    void stopLogger();

    /**
     * \brief Logs the given message with the given level.
     * \param level The level flag
     * \param message The message to log
     */
    void log(const std::string m, LogLevel l);
}