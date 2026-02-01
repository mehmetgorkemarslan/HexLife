#include <iostream>

#include "Config.h"
#include "Logger.h"
#include "mapGen/MapSystem/HexGrid.h"

// TODO: Create a test class
void LoggerTest() {
    Logger::Log(LogLevel::ERROR, "Error test message");
    Logger::Log(LogLevel::DEBUG, "this message is buffered");
    Logger::Log(LogLevel::INFO, "This message too");
    Logger::Log(LogLevel::FATAL, "This prints directly");
}

void MapGenTest() {
    auto *grid = new mapGen::HexGrid();

    grid->generateWorld(5);
    grid->displayGrid();
    grid->displayDisc({0, 1, -1}, 3);

    delete grid;
}

int main() {
    Config::getInstance().load();
    Logger::Init(Config::get<std::string>("log_file_name"));

    LoggerTest();
    MapGenTest();

    // Flush Buffer at end of every loop
    Logger::FlushBuffer();

    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}