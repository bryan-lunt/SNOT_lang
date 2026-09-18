cmake_minimum_required(VERSION 3.20)

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

option(ENABLE_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" ON)
if(ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -fPIC -g -O0)
    add_link_options(-fsanitize=address,undefined)
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


#A target to just use the param_storage.h header
add_library(snot_param_storage INTERFACE)
add_library(SNOT::storage ALIAS snot_param_storage)
target_sources(snot_param_storage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/param_storage.h)
target_include_directories(snot_param_storage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

################ PARSER RELATED ######################
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(snot_parser snot.ypp ${CMAKE_CURRENT_BINARY_DIR}/snot.tab.cpp
                      DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/snot.tab.hpp)

FLEX_TARGET(snot_lexer snot.lpp ${CMAKE_CURRENT_BINARY_DIR}/snot.yy.cpp)


ADD_FLEX_BISON_DEPENDENCY(snot_lexer snot_parser)

set_source_files_properties(${BISON_snot_parser_OUTPUTS}
    PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/param_storage.h"
)

set_source_files_properties(${FLEX_snot_lexer_OUTPUTS}
    PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/param_storage.h"
)

add_library(snot STATIC snot.h ${BISON_snot_parser_OUTPUTS} ${FLEX_snot_lexer_OUTPUTS})
set_target_properties(snot PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(snot PUBLIC snot_param_storage)
target_include_directories(snot INTERFACE
    ${CMAKE_CURRENT_BINARY_DIR}      # for the generated snot.yy.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}      # for param_storage.h
)

#The parser
#add_library(snot.tab OBJECT snot.tab.cpp snot.tab.hpp snot.yy.hpp param_storage.h)
#add_library(snot.lex OBJECT snot.lex.cpp snot.yy.hpp param_storage.h)

#add_library(snot STATIC snot.h $<TARGET_OBJECTS:snot.tab> $<TARGET_OBJECTS:snot.lex>)
#target_link_libraries(snot snot_param_storage)
#target_include_directories(snot INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
