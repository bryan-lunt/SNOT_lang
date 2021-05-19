cmake_minimum_required(VERSION 2.8)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")

#Tests the parser
add_executable(test1.exe ./tests/test1.cpp)
target_link_libraries(test1.exe snot_param_storage)
target_link_libraries(test1.exe snot)

#shows off the storage object API
add_executable(example1.exe ./examples/example1.cpp)
target_link_libraries(example1.exe snot_param_storage)

#another test program
add_executable(testprog.exe ./examples/testprog.cpp)
target_link_libraries(testprog.exe snot_param_storage)
