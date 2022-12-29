cmake_minimum_required(VERSION 3.0.0)
project(SDL2_Workbench VERSION 0.1.0)

set(CMAKE_SUPPRESS_REGENERATION true)

# include(CTest)
# enable_testing()

add_executable(SDL2_Workbench src/glad.c src/main.cpp)

# add_compile_definitions(GLEW_STATIC)

target_include_directories( ${PROJECT_NAME} PUBLIC src/ )

# target_link_libraries(SDL2_Workbench SDL2main SDL2 GL GLEW)
target_link_libraries(SDL2_Workbench SDL2main SDL2)
target_link_directories( ${PROJECT_NAME} PRIVATE lib/ )

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
