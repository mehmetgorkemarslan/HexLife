#include "Config.h"

void Config::load(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        Logger::Log(LogLevel::ERROR, "Config file can not found: " + path);
        return;
    }
    try {
        file >> getInstance()._data;
        Logger::Log(LogLevel::INFO, "Settings loaded: " + path);
    } catch (const json::parse_error &e) {
        Logger::Log(LogLevel::ERROR, "Json format error: " + std::string(e.what()));
    }
}
