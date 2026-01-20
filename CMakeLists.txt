cmake_minimum_required(VERSION 3.15)
project(HexLife VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/neat)
# Uncomment after including a separate CMakeLists.txt in src/map_gen.
#add_subdirectory(src/map_gen)

add_executable(HexLife
        src/main.cpp
)

target_link_libraries(HexLife PRIVATE neat map_gen)