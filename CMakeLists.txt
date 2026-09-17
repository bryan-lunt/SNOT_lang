cmake_minimum_required(VERSION 3.20)

project(SNOT
        VERSION 0.2.0
        LANGUAGES CXX
)

if(NOT DEFINED PROJECT_IS_TOP_LEVEL)
    if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
        set(PROJECT_IS_TOP_LEVEL TRUE)
    else()
        set(PROJECT_IS_TOP_LEVEL FALSE)
    endif()
endif()

add_subdirectory("lexyacc")
