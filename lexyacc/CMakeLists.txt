cmake_minimum_required(VERSION 2.8)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(test1.exe ./tests/test1.cpp)
target_link_libraries(test1.exe snot_param_storage)
target_link_libraries(test1.exe snot)
