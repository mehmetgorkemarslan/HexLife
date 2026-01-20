#pragma once
#include <iostream>
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
    static void Init(const std::string& filename = "simulation.log");
    static void Log(LogLevel level, const std::string& message);
private:
    Logger() = default;
    ~Logger();
    static Logger& getInstance();
    std::ofstream logFile;
};