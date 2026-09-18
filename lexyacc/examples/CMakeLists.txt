#shows off the storage object API
add_executable(example1.exe "example1.cpp")
target_link_libraries(example1.exe snot_param_storage)

#another test program
add_executable(testprog.exe "testprog.cpp")
target_link_libraries(testprog.exe snot_param_storage)

#another test program
add_executable(testprog_template.exe "testprog.cpp")
target_link_libraries(testprog_template.exe snot_param_storage)
