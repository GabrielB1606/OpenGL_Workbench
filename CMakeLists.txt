cmake_minimum_required(VERSION 3.0.0)
project(SDL2_Workbench VERSION 0.1.0)

set(CMAKE_SUPPRESS_REGENERATION true)

# main project
add_executable(${PROJECT_NAME} vendor/src/glad.c src/main.cpp)

# directories
target_link_directories( ${PROJECT_NAME} PRIVATE vendor/lib/ )
target_include_directories( ${PROJECT_NAME} PUBLIC vendor/include)

# mingw compiler additional stuff
if(MINGW)

    # mingw32 lib
target_link_libraries(SDL2_Workbench mingw32)

    # copy paste windows dll
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/dll/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/ )

endif(MINGW)

# libraries
target_link_libraries(${PROJECT_NAME} SDL2main SDL2)


