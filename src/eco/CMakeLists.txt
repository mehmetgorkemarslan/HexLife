add_library(eco STATIC
        Entity.h
		Species.h
)

target_sources(eco PRIVATE
        Entity.cpp
		PopulationManager.cpp
)

target_include_directories(eco
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(eco PUBLIC mapGen neat)

target_compile_features(eco PUBLIC cxx_std_20)