#ifndef HEADERS_H
#define HEADERS_H

// C++ Standard Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <filesystem>

// Graphic User Interface
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// File Input
// #include <noc_file_dialog.h>

// OpenGL Loader
#include <glad/glad.h>

// Window Manager
#include <GLFW/glfw3.h>

//OpenGL Math Library
#include <glm-0.9.9.9/vec3.hpp> // glm::vec3
#include <glm-0.9.9.9/vec4.hpp> // glm::vec4
#include <glm-0.9.9.9/mat4x4.hpp> // glm::mat4
#include <glm-0.9.9.9/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm-0.9.9.9/gtc/type_ptr.hpp>

// Load Images
#include <stb_image.h>

// Open File Dialog
// #include <noc_file_dialog.h>

// Assets Importer
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/DefaultLogger.hpp>
// assimp Flags
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices)

#define ARRAY_SIZE_IN_ELEMENTS(arr) (sizeof(arr)/sizeof(arr[0]))

#include "ShaderTypeEnum.h"

#define CORE_PROGRAM    0
#define SKYBOX_PROGRAM  1
#define SHADOW_PASS     2
#define LIGHT_PASS      3

#define SHADOW_MAP_SIZE 2048

#endif