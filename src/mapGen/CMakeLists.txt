add_library(mapGen STATIC
        # --- Common ---
        ./Common.h

        # --- MapSystem ---
        MapSystem/Hex.h
        MapSystem/Cell.h
        MapSystem/HexGrid.h
        MapSystem/WorldManager.h
        MapSystem/MapVisualizer.h

        # --- Noise ---
        Noise/NoiseGenerator.h
        Noise/PerlinNoise.h

        # --- Environment ---
        Environment/BiomeHandler.h
)

target_sources(mapGen PRIVATE
        # --- MapSystem ---
        MapSystem/Hex.cpp
        MapSystem/Cell.cpp
        MapSystem/HexGrid.cpp
        MapSystem/WorldManager.cpp
        MapSystem/MapVisualizer.cpp

        # --- Noise ---
        Noise/PerlinNoise.cpp

        # --- Environment ---
        Environment/BiomeHandler.cpp
)

target_include_directories(mapGen
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}

        ${CMAKE_CURRENT_SOURCE_DIR}/MapSystem
        ${CMAKE_CURRENT_SOURCE_DIR}/Noise
        ${CMAKE_CURRENT_SOURCE_DIR}/Environment
)

target_compile_features(mapGen PUBLIC cxx_std_20)