cmake_minimum_required(VERSION 3.0.0)
project(OpenGL_Workbench VERSION 0.1.0)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build Assimp test suite")
option(ASSIMP_NO_EXPORT "Disable exporting of symbols from the Assimp library" ON)
option(ASSIMP_BUILD_TESTS "Build the Assimp test suite" OFF)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Install Assimp PDB files")
# option(BUILD_SHARED_LIBS "Build the shared library version of Assimp" OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS "Build the Assimp command-line tools" OFF)
set(ASSIMP_BUILD_AMF_IMPORTER OFF)
set(ASSIMP_BUILD_AMF_EXPORTER OFF)
set(ASSIMP_BUILD_AC_IMPORTER OFF)
set(ASSIMP_BUILD_AC_EXPORTER OFF)
set(ASSIMP_BUILD_ASE_IMPORTER OFF)
set(ASSIMP_BUILD_ASE_EXPORTER OFF)
set(ASSIMP_BUILD_ASSBIN_IMPORTER OFF)
set(ASSIMP_BUILD_ASSBIN_EXPORTER OFF)
set(ASSIMP_BUILD_B3D_IMPORTER OFF)
set(ASSIMP_BUILD_B3D_EXPORTER OFF)
set(ASSIMP_BUILD_BVH_IMPORTER OFF)
set(ASSIMP_BUILD_BVH_EXPORTER OFF)
set(ASSIMP_BUILD_COLLADA_IMPORTER OFF)
set(ASSIMP_BUILD_COLLADA_EXPORTER OFF)
set(ASSIMP_BUILD_DXF_IMPORTER OFF)
set(ASSIMP_BUILD_DXF_EXPORTER OFF)
set(ASSIMP_BUILD_CSM_IMPORTER OFF)
set(ASSIMP_BUILD_CSM_EXPORTER OFF)
set(ASSIMP_BUILD_HMP_IMPORTER OFF)
set(ASSIMP_BUILD_HMP_EXPORTER OFF)
set(ASSIMP_BUILD_IRRMESH_IMPORTER OFF)
set(ASSIMP_BUILD_IRRMESH_EXPORTER OFF)
set(ASSIMP_BUILD_IQM_IMPORTER OFF)
set(ASSIMP_BUILD_IQM_EXPORTER OFF)
set(ASSIMP_BUILD_IRR_IMPORTER OFF)
set(ASSIMP_BUILD_IRR_EXPORTER OFF)
set(ASSIMP_BUILD_LWO_IMPORTER OFF)
set(ASSIMP_BUILD_LWO_EXPORTER OFF)
set(ASSIMP_BUILD_LWS_IMPORTER OFF)
set(ASSIMP_BUILD_LWS_EXPORTER OFF)
set(ASSIMP_BUILD_M3D_IMPORTER OFF)
set(ASSIMP_BUILD_M3D_EXPORTER OFF)
set(ASSIMP_BUILD_MD2_IMPORTER OFF)
set(ASSIMP_BUILD_MD2_EXPORTER OFF)
set(ASSIMP_BUILD_MD3_IMPORTER OFF)
set(ASSIMP_BUILD_MD3_EXPORTER OFF)
set(ASSIMP_BUILD_MD5_IMPORTER OFF)
set(ASSIMP_BUILD_MD5_EXPORTER OFF)
set(ASSIMP_BUILD_MDC_IMPORTER OFF)
set(ASSIMP_BUILD_MDC_EXPORTER OFF)
set(ASSIMP_BUILD_MDL_IMPORTER OFF)
set(ASSIMP_BUILD_MDL_EXPORTER OFF)
set(ASSIMP_BUILD_NFF_IMPORTER OFF)
set(ASSIMP_BUILD_NFF_EXPORTER OFF)
set(ASSIMP_BUILD_NDO_IMPORTER OFF)
set(ASSIMP_BUILD_NDO_EXPORTER OFF)
set(ASSIMP_BUILD_OFF_IMPORTER OFF)
set(ASSIMP_BUILD_OFF_EXPORTER OFF)
set(ASSIMP_BUILD_BLEND_IMPORTER OFF)
set(ASSIMP_BUILD_BLEND_EXPORTER OFF)
set(ASSIMP_BUILD_3DS_IMPORTER OFF)
set(ASSIMP_BUILD_3DS_EXPORTER OFF)
set(ASSIMP_BUILD_OGRE_IMPORTER OFF)
set(ASSIMP_BUILD_OGRE_EXPORTER OFF)
set(ASSIMP_BUILD_OPENGEX_IMPORTER OFF)
set(ASSIMP_BUILD_OPENGEX_EXPORTER OFF)
set(ASSIMP_BUILD_PLY_IMPORTER OFF)
set(ASSIMP_BUILD_PLY_EXPORTER OFF)
set(ASSIMP_BUILD_MS3D_IMPORTER OFF)
set(ASSIMP_BUILD_MS3D_EXPORTER OFF)
set(ASSIMP_BUILD_COB_IMPORTER OFF)
set(ASSIMP_BUILD_COB_EXPORTER OFF)
set(ASSIMP_BUILD_IFC_IMPORTER OFF)
set(ASSIMP_BUILD_IFC_EXPORTER OFF)
set(ASSIMP_BUILD_XGL_IMPORTER OFF)
set(ASSIMP_BUILD_XGL_EXPORTER OFF)
set(ASSIMP_BUILD_Q3D_IMPORTER OFF)
set(ASSIMP_BUILD_Q3D_EXPORTER OFF)
set(ASSIMP_BUILD_Q3BSP_IMPORTER OFF)
set(ASSIMP_BUILD_Q3BSP_EXPORTER OFF)
set(ASSIMP_BUILD_RAW_IMPORTER OFF)
set(ASSIMP_BUILD_RAW_EXPORTER OFF)
set(ASSIMP_BUILD_SIB_IMPORTER OFF)
set(ASSIMP_BUILD_SIB_EXPORTER OFF)
set(ASSIMP_BUILD_SMD_IMPORTER OFF)
set(ASSIMP_BUILD_SMD_EXPORTER OFF)
set(ASSIMP_BUILD_STL_IMPORTER OFF)
set(ASSIMP_BUILD_STL_EXPORTER OFF)
set(ASSIMP_BUILD_TERRAGEN_IMPORTER OFF)
set(ASSIMP_BUILD_TERRAGEN_EXPORTER OFF)
set(ASSIMP_BUILD_3D_IMPORTER OFF)
set(ASSIMP_BUILD_3D_EXPORTER OFF)
set(ASSIMP_BUILD_X_IMPORTER OFF)
set(ASSIMP_BUILD_X_EXPORTER OFF)
set(ASSIMP_BUILD_X3D_IMPORTER OFF)
set(ASSIMP_BUILD_X3D_EXPORTER OFF)
set(ASSIMP_BUILD_3MF_IMPORTER OFF)
set(ASSIMP_BUILD_3MF_EXPORTER OFF)
set(ASSIMP_BUILD_MMD_IMPORTER OFF)
set(ASSIMP_BUILD_MMD_EXPORTER OFF)
add_subdirectory(./vendor/assimp)

set(CMAKE_SUPPRESS_REGENERATION true)

# all imgui .cpp files
file( GLOB IMGUI_SRC vendor/imgui-docking-1.89.3/*.cpp )
file( GLOB IMGUI_BACKEND_SRC vendor/imgui-docking-1.89.3/backends/*.cpp )
file( GLOB PROJECT_SRC src/*.cpp )

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
# 32 bits
    set(GLFW_DIR vendor/glfw-3.3.8.bin.WIN32)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
# 64 bits
    set(GLFW_DIR vendor/glfw-3.3.8.bin.WIN64)
endif()

if(MINGW)
    set(GLFW_COMPILER lib-mingw)
    # Set linker flags
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
elseif(MINGW_W64)
    set(GLFW_COMPILER lib-mingw-w64)
    # Set linker flags
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
elseif(MSVC)

    # if(CMAKE_GENERATOR_TOOLSET MATCHES "v100")
    #     set(GLFW_COMPILER lib-vc2010)

    # elseif(CMAKE_GENERATOR_TOOLSET MATCHES "v110")
    #     set(GLFW_COMPILER lib-vc2012)

    # elseif(CMAKE_GENERATOR_TOOLSET MATCHES "v120")
    #     set(GLFW_COMPILER lib-vc2013)

    # elseif(CMAKE_GENERATOR_TOOLSET MATCHES "v140")
    #     set(GLFW_COMPILER lib-vc2015)

    # elseif(CMAKE_GENERATOR_TOOLSET MATCHES "v141")
    #     set(GLFW_COMPILER lib-vc2017)

    # elseif(CMAKE_GENERATOR_TOOLSET MATCHES "v142")
    #     set(GLFW_COMPILER lib-vc2019)

    # elseif(CMAKE_GENERATOR_TOOLSET MATCHES "v143")
    
    # endif()
    set(GLFW_COMPILER lib-vc2022)

else()
    set(GLFW_COMPILER lib-static-ucrt)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug mode
    set(GLAD_DIR vendor/glad-opengl-4.6-debug)
else()
    # Release mode
    set(GLAD_DIR vendor/glad-opengl-4.6-release)
endif()

# main project
add_executable(${PROJECT_NAME}
    ${GLAD_DIR}/src/glad.c
    ${IMGUI_SRC}
    ${IMGUI_BACKEND_SRC}
    src/stb_image.cpp
    src/stb_particle_system.cpp
    src/noc_file_dialog.cpp
    src/WindowManager.cpp
    src/ShaderProgram.cpp
    src/Texture.cpp
    src/Material.cpp
    src/ShadowCubeMapFBO.cpp
    src/CubeMapFBO.cpp
    src/SceneFBO.cpp
    src/Transformable.cpp
    src/BasicMesh.cpp
    src/Light.cpp
    src/ViewCamera.cpp
    src/Skybox.cpp
    src/Plane.cpp
    src/World.cpp
    src/GraphicUserInterface.cpp
    src/inputProcessor.cpp
    main.cpp
)

# directories
target_include_directories( ${PROJECT_NAME} PUBLIC include/headers/)
target_include_directories( ${PROJECT_NAME} PUBLIC include/ ${GLAD_DIR}/include vendor/imgui-docking-1.89.3 vendor/imgui-docking-1.89.3/backends ${GLFW_DIR}/include vendor/ vendor/assimp/include build/vendor/assimp/include)

target_link_directories( ${PROJECT_NAME} PUBLIC ${GLFW_DIR}/${GLFW_COMPILER} build/vendor/assimp/lib )

# Set compiler flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Set the output directories
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(assimp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add_compile_definitions(SDL_VIDEO_DRIVER_WINDOWS)
    
    # endif(MINGW)
    
    # libraries
target_link_libraries(${PROJECT_NAME} glfw3dll opengl32 assimp)
    # target_link_libraries(${PROJECT_NAME} mingw32 glfw3dll opengl32 gdi32 user32 assimp)
    
    # copy paste windows dll
add_custom_command(
TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy
${CMAKE_SOURCE_DIR}/${GLFW_DIR}/${GLFW_COMPILER}/glfw3.dll
$<TARGET_FILE_DIR:${PROJECT_NAME}>/ )

# copy paste windows dll
add_custom_command(
TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy
${CMAKE_SOURCE_DIR}/build/vendor/assimp/bin/libassimp-5d.dll
$<TARGET_FILE_DIR:${PROJECT_NAME}>/ )

    # copy paste shaders
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/ )

    # copy paste shaders
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/models/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/models/ )
