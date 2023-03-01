#ifndef HEADERS_H
#define HEADERS_H

// C++ Standard Libraries
#include <iostream>
#include <fstream>
#include <string>

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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OpenglLoader.h"
#include "WindowManager.h"

#include "GraphicUserInterface.h"

#include "ShaderTypeEnum.h"
#include "ShaderProgram.h"

#endif