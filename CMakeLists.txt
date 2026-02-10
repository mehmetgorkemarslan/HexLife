cmake_minimum_required(VERSION 3.15)
project(HexLife VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Can not compile Neat. There are 2 NodeType enum. One in Gene other in Node
add_subdirectory(src/neat)
add_subdirectory(src/mapGen)

add_executable(HexLife
        src/main.cpp
        src/utils/Logger.cpp
        src/utils/Config.cpp
)

target_include_directories(HexLife PRIVATE include)


# Can not compile Neat. There is 2 NodeType enum. One in Gene other in Node
target_link_libraries(HexLife PRIVATE  mapGen)