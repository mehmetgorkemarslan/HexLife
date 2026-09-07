cmake_minimum_required(VERSION 3.15)
project(HexLife VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/neat)
add_subdirectory(src/mapGen)
add_subdirectory(src/eco)

find_package(raylib REQUIRED)

add_executable(HexLife
        src/main.cpp
        src/Test.cpp
        src/SimulationWorld.cpp
        include/SimulationWorld.h
        src/visualization/SimulationRenderer.cpp
        include/SimulationRenderer.h
        src/utils/Logger.cpp
        src/utils/Config.cpp
)

target_include_directories(HexLife PRIVATE include)


target_link_libraries(HexLife PRIVATE mapGen)
target_link_libraries(HexLife PRIVATE neat)
target_link_libraries(HexLife PRIVATE eco)
target_link_libraries(HexLife PRIVATE raylib)