#include "../include/Logger.h"

Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::Init(const std::string &filename) {
    getInstance().logFile.open(filename, std::ios::app);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::Log(LogLevel level, const std::string &message) {
    std::string levelStr;
    switch (level) {
        case LogLevel::DEBUG: levelStr = "[DEBUG]"; break;
        case LogLevel::INFO: levelStr = "[INFO]"; break;
        case LogLevel::WARNING: levelStr = "[WARNING]"; break;
        case LogLevel::ERROR: levelStr = "[ERROR]"; break;
        case LogLevel::FATAL: levelStr = "[FATAL]"; break;
    }

    auto now = std::chrono::system_clock::now();

    std::string logEntry = std::format("{:%Y-%m-%d %H:%M:%S} {} {}", now, levelStr, message);
    std::cout << logEntry << std::endl;
    if (getInstance().logFile.is_open()) {
        getInstance().logFile << logEntry << std::endl;
    }
}
