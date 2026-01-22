add_library(neat STATIC
        phenotype/Node.h
        phenotype/Connection.h
        phenotype/Network.h
        genotype/Gene.h
        genotype/Genome.h)

target_sources(neat PRIVATE
        phenotype/Network.cpp
)

target_include_directories(neat
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(neat PUBLIC cxx_std_20)