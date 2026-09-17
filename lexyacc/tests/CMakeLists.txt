#Tests the parser
add_executable(test1.exe "test1.cpp")
target_link_libraries(test1.exe snot_param_storage)
target_link_libraries(test1.exe snot)
