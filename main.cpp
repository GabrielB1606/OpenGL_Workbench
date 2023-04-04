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
ViewCamera mainCamera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f) );

// Input Processor
InputProcessor input(&windowManager);

// setup the GUI
GraphicUserInterface gui(&windowManager, glMajVersion, glMinVersion);

void configOpenGL();
void updateProjectionViewMatrix();

int main() {
	
	// initial configuration
	configOpenGL();

	// background color
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	// load a model
	w.loadMesh("models\\Crate1.obj");
	w.loadMesh("models\\Crate1.obj");
	
	// translate models
	w.getMeshes()[0]->translate(glm::vec3(0.f, 0.f, 7.f) );
	w.getMeshes()[1]->translate(glm::vec3(1.f, 1.f, 2.5f));

	// add default light
	w.addLight( new Light(glm::vec3(0.f)) );

	// time between frames
	float delta = 0.001f;

	// skybox setup
	w.createSkybox(shaderPrograms[SKYBOX_PROGRAM], "shaders/skybox/sky/", "jpg");
	shaderPrograms[SKYBOX_PROGRAM]->setMat4fv(w.getPerspectiveMatrix(), "ProjectionMatrix", GL_FALSE);

	// send uniforms
	mainCamera.sendUniforms( shaderPrograms[CORE_PROGRAM] );
	mainCamera.sendUniforms( shaderPrograms[LIGHT_PASS] );
	updateProjectionViewMatrix();

	// start counting time between frames
	windowManager.getDeltaTime();

	while ( windowManager.isOpen() ) {
		
		// rotate just bc
		w.getMeshes()[0]->rotate( delta*glm::vec3(30.f, 30.f, 30.f) );

		// Only if there's camera movement, send the view matrix again
		if(input.process(&mainCamera, delta)){
			mainCamera.sendUniforms( shaderPrograms[CORE_PROGRAM] );
			mainCamera.sendUniforms( shaderPrograms[LIGHT_PASS] );
			updateProjectionViewMatrix();
		}

		// these are pretty much light uniforms
		w.sendUniforms(shaderPrograms[CORE_PROGRAM]);
		w.sendUniforms( shaderPrograms[LIGHT_PASS] );

		w.renderShadowCubeMaps(shaderPrograms[SHADOW_PASS]);

		// clear main framebuffer
		glViewport(0, 0, (int)w.getWidth(), (int)w.getHeight());
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		// render meshes
		w.renderMeshes(shaderPrograms[LIGHT_PASS]);
		
		// render skybox
		w.renderSkybox( mainCamera.getViewMatrix() );

		// render GUI
		gui.draw(&w, &clear_color);
		gui.render();

		// end frame
		windowManager.swapBuffers();
		delta = windowManager.getDeltaTime();

	}

	// delete all shader programs
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
	
	// Face culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

}

void updateProjectionViewMatrix(){

	glm::mat4 ProjView = w.getPerspectiveMatrix() * mainCamera.getViewMatrix();
	
	shaderPrograms[CORE_PROGRAM]->setMat4fv(ProjView, "ProjViewMatrix", GL_FALSE);
	shaderPrograms[LIGHT_PASS]->setMat4fv(ProjView, "ProjViewMatrix", GL_FALSE);

}

void updateProjectionMatrix(){

	shaderPrograms[SKYBOX_PROGRAM]->setMat4fv(w.getPerspectiveMatrix(), "ProjViewMatrix", GL_FALSE);
	shaderPrograms[SHADOW_PASS]->setMat4fv(w.getPerspectiveMatrix(), "ProjViewMatrix", GL_FALSE);

}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	
	glViewport(0, 0, width, height);
	w.setAspectRatio((float)width, (float)height);

	updateProjectionMatrix();
	updateProjectionViewMatrix();

}