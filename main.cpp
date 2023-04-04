#include "headers.h"

#include "OpenglLoader.h"
#include "WindowManager.h"
#include "ViewCamera.h"
#include "ShaderProgram.h"
#include "Light.h"
#include "Skybox.h"
#include "World.h"
#include "BasicMesh.h"
#include "InputProcessor.h"
#include "GraphicUserInterface.h"

// GLSL Version
const short glMajVersion = 4, glMinVersion = 6;
std::string glVersion_str = std::to_string(glMajVersion) + std::to_string(glMinVersion) + "0";

// Define the current state of the 3D world
World w(90.f, 1280, 720, 0.1f, 1000.f);

// Manage window creation
WindowManager windowManager(1280, 720, "window manager", glMajVersion, glMinVersion);

// All shader programs used in the application
std::array<ShaderProgram*, 4> shaderPrograms = {
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/core/vertex.vert", "shaders/core/fragment.frag"),
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/skybox/skybox.vert", "shaders/skybox/skybox.frag"),
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/shadow/shadow_pass.vert", "shaders/shadow/shadow_pass.frag"),
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/shadow/light_pass.vert", "shaders/shadow/light_pass.frag")
};

// camera
ViewCamera cam(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f) );

// Input Processor
InputProcessor input(&windowManager);

// setup the GUI
GraphicUserInterface gui(&windowManager, glMajVersion, glMinVersion);

void configOpenGL();

int main() {
	
	// initial configuration
	configOpenGL();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	shaderPrograms[CORE_PROGRAM]->setMat4fv(w.getPerspectiveMatrix(), "ProjectionMatrix", GL_FALSE);
	shaderPrograms[SKYBOX_PROGRAM]->setMat4fv(w.getPerspectiveMatrix(), "ProjectionMatrix", GL_FALSE);

	// load a model
	w.loadMesh("models\\Crate1.obj");
	w.loadMesh("models\\Crate1.obj");
	
	// translate models
	w.getMeshes()[0]->translate( glm::vec3(0.f, 0.f, 7.f) );
	w.getMeshes()[1]->translate(glm::vec3(1.f, 1.f, 2.5f));

	// add default light
	w.addLight( new Light(glm::vec3(0.f)) );

	// time between frames
	float delta = 0.001f;

	w.createSkybox(shaderPrograms[SKYBOX_PROGRAM], "shaders/skybox/sky/", "jpg");
	cam.sendUniforms( shaderPrograms[CORE_PROGRAM] );

	windowManager.getDeltaTime();

	while ( windowManager.isOpen() ) {
		
		w.getMeshes()[0]->rotate( delta*glm::vec3(30.f, 30.f, 30.f) );

		// Only if there's camera movement, send the view matrix again
		if(input.process(&cam, delta)){
			cam.sendUniforms( shaderPrograms[CORE_PROGRAM] );
		}

		w.sendUniforms(shaderPrograms[CORE_PROGRAM]);

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		w.renderMeshes(shaderPrograms[CORE_PROGRAM]);
		w.renderSkybox( cam.getViewMatrix() );

		gui.draw(&w, &clear_color);
		gui.render();

		windowManager.swapBuffers();
		delta = windowManager.getDeltaTime();

	}

	for( ShaderProgram* sp : shaderPrograms )
		delete sp;

	return 0;
}

void configOpenGL(){

	// Enable multisample anti-aliasing (MSAA)
	glEnable(GL_MULTISAMPLE);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// Specify the depth function
	glDepthFunc(GL_LESS);
	// glDepthFunc(GL_LEQUAL);
	
	// Face culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	
	glViewport(0, 0, width, height);
	w.setAspectRatio((float)width, (float)height);

	for( ShaderProgram* sp : shaderPrograms )
		sp->setMat4fv(w.getPerspectiveMatrix(), "ProjectionMatrix", GL_FALSE);

}