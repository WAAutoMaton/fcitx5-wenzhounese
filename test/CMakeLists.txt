configure_file(testdir.h.in ${CMAKE_CURRENT_BINARY_DIR}/testdir.h @ONLY)

foreach(TESTCASE testencoder testdecoder testdictionary)
    add_executable(${TESTCASE} ${TESTCASE}.cpp)
    target_include_directories(${TESTCASE} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries(${TESTCASE} LibIME::Wenzhounese)
    add_test(NAME ${TESTCASE} COMMAND ${TESTCASE})
endforeach()
add_dependencies(testdecoder dict model)
add_dependencies(testdictionary dict)

add_executable(testime testime.cpp)
target_include_directories(testime PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(testime LibIME::Wenzhounese)
add_dependencies(testime dict model)

add_executable(testdictionaryexport testdictionaryexport.cpp)
target_link_libraries(testdictionaryexport LibIME::Wenzhounese)
add_test(NAME testdictionaryexport COMMAND testdictionaryexport)
