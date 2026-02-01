add_library(mapGen STATIC
        MapSystem/Hex.h
        MapSystem/Cell.h
        MapSystem/HexGrid.h
)

target_sources(mapGen PRIVATE
        MapSystem/Hex.cpp
        MapSystem/Cell.cpp
        MapSystem/HexGrid.cpp
)

target_include_directories(mapGen
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(mapGen PUBLIC cxx_std_20)