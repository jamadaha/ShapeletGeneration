add_executable(Entropy ${CMAKE_CURRENT_LIST_DIR}/entropy.cpp) # Parameters : classes, expectedEntropy
target_link_libraries(Entropy
        PRIVATE
            ShapeletGeneration)
target_include_directories(Entropy
        PRIVATE
            src)

add_test(Entropy_Empty Entropy "" 0)
add_test(Entropy_SingleClass1 Entropy "0" 0)
add_test(Entropy_SingleClass2 Entropy "0 0" 0)
add_test(Entropy_SingleClass3 Entropy "1 1" 0)
add_test(Entropy_MultiClass1 Entropy "0 1" 1)
add_test(Entropy_MultiClass2 Entropy "0 0 1 1" 1)
add_test(Entropy_MultiClass3 Entropy "0 1 2" 1.58)
add_test(Entropy_MultiClass4 Entropy "0 0 1 2" 1.5)

add_executable(InformationGain ${CMAKE_CURRENT_LIST_DIR}/information_gain.cpp) # Parameters : classes, frequencies, expectedGain
target_link_libraries(InformationGain
        PRIVATE
        ShapeletGeneration)
target_include_directories(InformationGain
        PRIVATE
        src)

