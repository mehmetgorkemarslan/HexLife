#include <iostream>
#include "Logger.h"
int main() {
    std::cout << "Helo" << std::endl;

    Logger::Init("hexlife.log");
    Logger::Log(LogLevel::ERROR, "Error deneme");

    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}