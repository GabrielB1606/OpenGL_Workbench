#ifndef HEADERS_H
#define HEADERS_H

// C++ Standard Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Graphic User Interface
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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


// Assets Importer
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
// #ifndef STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_IMPLEMENTATION
//     #include <stb_image.h>
// #endif
// assimp Flags
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

#define ARRAY_SIZE_IN_ELEMENTS(arr) (sizeof(arr)/sizeof(arr[0]))

// #include <stb_image.h>


#include "OpenglLoader.h"
#include "WindowManager.h"

#include "GraphicUserInterface.h"

#include "Texture.h"
#include "Material.h"

#include "ShaderTypeEnum.h"
#include "ShaderProgram.h"

#include "BasicMeshEntry.h"
#include "BasicMesh.h"

#endif