#include "Logger.h"
#include <iostream>

void Logger::Init(const std::string &filename) {
    getInstance().logFile.open(filename, std::ios::app);
}

void Logger::Log(LogLevel level, const std::string &message) {
    std::string levelStr;
    bool bufferMessage = false;
    switch (level) {
        case LogLevel::DEBUG: levelStr = "[DEBUG]";
            bufferMessage = true;
            break;
        case LogLevel::INFO: levelStr = "[INFO]";
            bufferMessage = true;
            break;
        case LogLevel::WARNING: levelStr = "[WARNING]";
            break;
        case LogLevel::ERROR: levelStr = "[ERROR]";
            break;
        case LogLevel::FATAL: levelStr = "[FATAL]";
            break;
    }

    auto now = std::chrono::system_clock::now();

    std::string logEntry = std::format("{:%Y-%m-%d %H:%M:%S} {} {}\n", now, levelStr, message);
    if (!bufferMessage) {
        std::cout << logEntry;
        LogToFile(logEntry);
        return;
    }
    getInstance().buffer << logEntry;
}

void Logger::FlushBuffer() {
    std::string message = getInstance().buffer.str();
    std::cout << message;
    LogToFile(message);
    ClearBuffer();
}

void Logger::ClearBuffer() {
    getInstance().buffer.str("");
    getInstance().buffer.clear();
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::LogToFile(const std::string &message) {
    if (getInstance().logFile.is_open()) {
        getInstance().logFile << message;
    }
}
