#include <iostream>
#include "Logger.h"
#include "mapGen/MapSystem/HexGrid.h"

// TODO: Create a test class
void LoggerTest() {
    Logger::Log(LogLevel::ERROR, "Error deneme");
}

void MapGenTest() {
    auto *grid = new mapGen::HexGrid();

    grid->generateWorld(5);
    grid->displayGrid();
    grid->displayDisc({0, 1, -1}, 3);

    delete grid;
}

int main() {
    Logger::Init("hexlife.log");

    LoggerTest();
    MapGenTest();

    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}