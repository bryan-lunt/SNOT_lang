cmake_minimum_required(VERSION 2.8)

########## CMAKE SETUP ###########
#For building on much newer CMAKE
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    )
#include("PreventInTreeBuilds")



#
######### PROJECT ###########
# set the project name
project(SNOT_lang VERSION 0.1.0 LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


#A target to just use the param_storage.h header
add_library(snot_param_storage INTERFACE)
target_sources(snot_param_storage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/param_storage.h)
target_include_directories(snot_param_storage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(snot.tab OBJECT snot.tab.cpp snot.tab.hpp snot.yy.hpp param_storage.h)
add_library(snot.lex OBJECT snot.lex.cpp snot.yy.hpp param_storage.h)

add_library(snot STATIC snot.h $<TARGET_OBJECTS:snot.tab> $<TARGET_OBJECTS:snot.lex>)
target_link_libraries(snot snot_param_storage)
target_include_directories(snot INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
