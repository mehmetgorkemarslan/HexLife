#pragma once
#include <fstream>
#include <string>
#include <chrono>
#include <format>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger {
public:
    static void Init(const std::string &filename = "simulation.log");

    static void Log(LogLevel level, const std::string &message);

    static void FlushBuffer();

    static void ClearBuffer();

private:
    Logger() = default;

    ~Logger();

    static Logger &getInstance();

    static void LogToFile(const std::string &message);

    std::ofstream logFile;
    std::stringstream buffer;
};
