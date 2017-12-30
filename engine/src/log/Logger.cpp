#include "log/Logger.h"

#include <chrono>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

namespace engine::log {
    static int level;
    static int output;
    static std::ofstream logFile;

    const std::string createHeader() {
        std::stringstream header;
        for (unsigned int i = 0 ; i < 100 ; i++)
            header << "=";
        header << "\n";

        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        header << "NEW LOG:\n";
        header << "Starting at: " << std::put_time(std::localtime(&now), "%Ex %EX") << "\n";
        
        header.flush();
        return header.str();
    }

    void startLogger(int l, int o) {
        setLogLevel(l);
        setLogOutput(o);

        logFile.open("media/log.txt");

        auto header = createHeader();
        
        if (output & LogOutput::Console) {
            std::cout << header;
        }
        if (output & LogOutput::File) {
            logFile << header;
        }
    }

    void stopLogger() {
        log("Stopping the logger", log::LogLevel::Info);
        logFile.close();
    }
        
    void setLogLevel(int l) {
        log("Changing log level to " + std::to_string(l), log::LogLevel::Info);
        level = l;
    }

    void setLogOutput(int o) {
        log("Changing log output to " + std::to_string(o), log::LogLevel::Info);
        output = o;
    }

    void log(const std::string m, LogLevel l) {
        // If the levels match
        if (level & level) {
            std::stringstream out;
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

            out << std::put_time(std::localtime(&now), "%T") << "\t";

            switch(l){
            case LogLevel::LevelNone:
                return;
            case LogLevel::Info:
                out << "[INFO]:\t";
                break;
            case LogLevel::Warning:
                out << "[WARNING]:\t";
                break;
            case LogLevel::Error:
                out << "[ERROR]:\t";
                break;
            default:
                log("A log message must be of type Info OR Warning OR Error", LogLevel::Error);
                break;
            }

            out << m<< "\n";
            out.flush();

            if (output & LogOutput::Console) {
                if (l == LogLevel::Error)
                    std::cerr << out.str();
                else
                    std::cout << out.str();
            }
            if (output & LogOutput::File) {
                logFile << out.str();
            }
        }
    }
}