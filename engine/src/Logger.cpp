#include "SkellEngine/Logger.h"

#include <iostream>

namespace engine {
    Logger::Logger() noexcept :
        m_output(LogOutput::Console),
        m_level(LogType::Info) {

    }

    void Logger::setLogLevel(LogType level) noexcept {
        m_level = level;
    }

    void Logger::setOutputFile(const std::filesystem::path &path) noexcept {
        try {
            m_outputFile.open(path);
        }
        catch (const std::ios_base::failure &e) {
            forceLog("Logger: impossible to open a new output file: " + std::string(e.what()));
        }
    }

    void Logger::setLogOutput(LogOutput output) noexcept {
        m_output = output;
    }

    void Logger::log(const std::string &message, LogType type) const noexcept {
        if (type >= m_level) {
            std::string prefix;
            switch (type) {
            case LogType::Info:
                prefix = "[INFO]";
                break;
            case LogType::Warning:
                prefix = "[WARNING]";
                break;
            case LogType::Error:
                prefix = "[ERROR]";
                break;
            }

            if (m_output == LogOutput::Console || m_output == LogOutput::All) {
                std::cout << prefix << " " << message << "\n";
            }
            if (m_output == LogOutput::File || m_output == LogOutput::All) {
                try {
                    m_outputFile << prefix << " " << message << "\n";
                    m_outputFile.flush();
                }
                catch (const std::ios_base::failure &e) {
                    forceLog("Logger: impossible to write to the log file. The message is " + message);
                }
            }
        }
    }

    void Logger::logError(const std::exception &exception) const noexcept {
        log(exception.what(), LogType::Error);
    }

    void Logger::logError(const std::string &message, const std::exception &exception) const noexcept {
        log(message + "\n" + exception.what(), LogType::Error);
    }

    void Logger::forceLog(const std::string &message) noexcept {
        std::cout << "[ERROR] " << message << "\n";
    }
}