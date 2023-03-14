add_executable(SingleSeries ${CMAKE_CURRENT_LIST_DIR}/single_series.cpp) # Parameters : seriesLength, windowSize, expectedWindowCount
target_link_libraries(SingleSeries
        PRIVATE
            ShapeletGeneration)
target_include_directories(SingleSeries
        PRIVATE
            src)

add_test(SingleSeries_Empty SingleSeries 0 0 0)
add_test(SingleSeries_1 SingleSeries 1 1 1)
add_test(SingleSeries_2 SingleSeries 2 1 2)
add_test(SingleSeries_3 SingleSeries 3 2 2)
