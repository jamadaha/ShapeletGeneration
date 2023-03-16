cmake_minimum_required(VERSION 3.22.1)
project(ShapeletGeneration)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_library(ShapeletGeneration
        src/InformationGain.hpp
        src/SlidingWindows.hpp
        src/Types.hpp
        src/Shapelets.hpp
        src/FileHandler.h
        src/attributes/Attribute.h src/attributes/Frequency.h src/SeriesActions.h)

add_executable(ShapeletGenerator ShapeletGenerator.cpp)
target_link_libraries(ShapeletGenerator PRIVATE ShapeletGeneration)
target_include_directories(ShapeletGenerator PRIVATE src)

set_target_properties(ShapeletGeneration PROPERTIES LINKER_LANGUAGE CXX)

include(test/CMakeLists.txt)