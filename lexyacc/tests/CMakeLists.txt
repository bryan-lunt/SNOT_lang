option(ENABLE_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" ON)
if(ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -fPIC -g -O0)
    add_link_options(-fsanitize=address,undefined)
endif()

#Tests the parser
add_executable(test1.exe "test1.cpp")
target_link_libraries(test1.exe PRIVATE snot_param_storage)
target_link_libraries(test1.exe PRIVATE snot)

add_executable(test2.exe "test2.cpp")
target_link_libraries(test2.exe PRIVATE snot_param_storage)

add_executable(test3.exe "test3.cpp")
target_link_libraries(test3.exe PRIVATE snot_param_storage)
target_link_libraries(test3.exe PRIVATE snot)
