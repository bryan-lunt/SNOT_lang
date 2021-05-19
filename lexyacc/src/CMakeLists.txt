
#A target to just use the param_storage.h header
add_library(snot_param_storage INTERFACE)
target_sources(snot_param_storage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/param_storage.h)
target_include_directories(snot_param_storage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(snot.tab OBJECT snot.tab.cpp snot.tab.hpp snot.yy.hpp param_storage.h)
add_library(snot.lex OBJECT snot.lex.cpp snot.yy.hpp param_storage.h)

add_library(snot STATIC snot.h $<TARGET_OBJECTS:snot.tab> $<TARGET_OBJECTS:snot.lex>)
target_link_libraries(snot snot_param_storage)
target_include_directories(snot INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
