add_executable(HelloApp src/main.cpp)
target_link_libraries(HelloApp PUBLIC HelloLib)

if(WIN32)
    add_custom_command(TARGET HelloApp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE_DIR:HelloLib>/HelloLib.dll $<TARGET_FILE_DIR:HelloApp>)
endif()