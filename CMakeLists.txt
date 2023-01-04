cmake_minimum_required(VERSION 3.0.0)
project(SDL2_Workbench VERSION 0.1.0)

set(CMAKE_SUPPRESS_REGENERATION true)

# all imgui .cpp files
file( GLOB IMGUI_SRC vendor/src/imgui/*.cpp )

# main project
add_executable(${PROJECT_NAME} vendor/src/glad/glad.c ${IMGUI_SRC} src/main.cpp)

# directories
target_link_directories( ${PROJECT_NAME} PRIVATE vendor/lib/ )
target_include_directories( ${PROJECT_NAME} PUBLIC include/ vendor/include vendor/include/imgui)

# mingw compiler additional stuff
if(MINGW)

    # mingw32 lib
target_link_libraries(${PROJECT_NAME} mingw32)

    # copy paste windows dll
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/dll/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/ )

add_compile_definitions(SDL_VIDEO_DRIVER_WINDOWS)

endif(MINGW)

# libraries
target_link_libraries(${PROJECT_NAME} SDL2main SDL2)
