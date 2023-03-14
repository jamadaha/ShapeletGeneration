cmake_minimum_required(VERSION 3.22.1)
project(ShapeletGeneration)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_library(ShapeletGeneration
        src/InformationGain.hpp
        src/SequenceMatching.hpp
        src/SlidingWindows.hpp
        src/Types.hpp
        src/Shapelets.hpp)

set_target_properties(ShapeletGeneration PROPERTIES LINKER_LANGUAGE CXX)

include(test/CMakeLists.txt)