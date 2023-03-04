cmake_minimum_required(VERSION 3.0.0)
project(OpenGL_Workbench VERSION 0.1.0)

add_subdirectory(./vendor/assimp)

set(CMAKE_SUPPRESS_REGENERATION true)

# all imgui .cpp files
file( GLOB IMGUI_SRC vendor/imgui-docking-1.89.3/*.cpp )
file( GLOB IMGUI_BACKEND_SRC vendor/imgui-docking-1.89.3/backends/*.cpp )
file( GLOB PROJECT_SRC src/*.cpp )

# main project
add_executable(${PROJECT_NAME} main.cpp vendor/glad-opengl-4.6-release/src/glad.c ${IMGUI_SRC} ${IMGUI_BACKEND_SRC} ${PROJECT_SRC} )

# directories
target_include_directories( ${PROJECT_NAME} PUBLIC include/ vendor/glad-opengl-4.6-release/include vendor/imgui-docking-1.89.3 vendor/imgui-docking-1.89.3/backends vendor/glfw-3.3.8.bin.WIN32/include vendor/ vendor/assimp/include)

target_link_directories( ${PROJECT_NAME} PRIVATE vendor/glfw-3.3.8.bin.WIN32/lib-mingw)
# mingw compiler additional stuff
# if(MINGW)


# mingw32 lib
# target_link_libraries(${PROJECT_NAME} mingw32)

    # copy paste windows dll
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/vendor/glfw-3.3.8.bin.WIN32/lib-mingw/glfw3.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/ )

# add_compile_definitions(SDL_VIDEO_DRIVER_WINDOWS)

# endif(MINGW)

# libraries
target_link_libraries(${PROJECT_NAME} mingw32 glfw3dll opengl32 gdi32 user32 assimp)
# target_link_libraries(${PROJECT_NAME} mingw32 glfw3dll opengl32 gdi32 user32 assimp)

    # copy paste shaders
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/ )
