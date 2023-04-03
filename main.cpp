#include "headers.h"

#include "OpenglLoader.h"
#include "WindowManager.h"
#include "ViewCamera.h"
#include "ShaderProgram.h"
#include "Light.h"
#include "Skybox.h"
#include "World.h"
#include "BasicMesh.h"
#include "GraphicUserInterface.h"

const short glMajVersion = 4, glMinVersion = 6;

// Define the projection matrix
World w(90.f, 1280, 720, 0.1f, 1000.f);

// create the window
WindowManager windowManager(1280, 720, "window manager", glMajVersion, glMinVersion);

// define shader program
std::array<ShaderProgram*, 2> shaderPrograms = {
	new ShaderProgram("460", glMajVersion, glMinVersion, "shaders/core/vertex.vert", "shaders/core/fragment.frag"),
	new ShaderProgram("460", glMajVersion, glMinVersion, "shaders/skybox/skybox.vert", "shaders/skybox/skybox.frag")
};

WindowManager::BTN_STATE lastStateRightBtn = WindowManager::BTN_STATE::RELEASE;
double mouseX, mouseY;
void processInput(std::unordered_set<std::string> input, WindowManager *window, ViewCamera *cam, float delta);

void configOpenGL();

int main() {
	
	// initial configuration
	configOpenGL();

	// setup the GUI
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	GraphicUserInterface gui(&windowManager, glMajVersion, glMinVersion);
	
	shaderPrograms[CORE_PROGRAM]->setMat4fv(w.getPerspectiveMatrix(), "ProjectionMatrix", GL_FALSE);
	shaderPrograms[SKYBOX_PROGRAM]->setMat4fv(w.getPerspectiveMatrix(), "ProjectionMatrix", GL_FALSE);

	w.loadMesh("models\\Crate1.obj");
	w.loadMesh("models\\Crate1.obj");

	// camera
	ViewCamera cam(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f) );

	// load a model
	BasicMesh* mesh = w.getMeshes()[0];
	mesh->translate( glm::vec3(0.f, 0.f, 5.f) );

	w.getMeshes()[1]->translate(glm::vec3(1.f, 1.f, 2.5f));

	w.addLight( new Light(glm::vec3(0.f)) );
	Light *light = w.getLights()[0];

	float delta = 0.001f;

	std::unordered_set<std::string> input;

	Skybox sb(shaderPrograms[SKYBOX_PROGRAM], "shaders/skybox/sky/", "jpg");

	windowManager.getDeltaTime();

	while ( windowManager.isOpen() ) {
		
		mesh->rotate( delta*glm::vec3(30.f, 30.f, 30.f) );

		input = windowManager.pollEvents();
		processInput(input, &windowManager, &cam, delta);

		shaderPrograms[CORE_PROGRAM]->setMat4fv(cam.getViewMatrix(), "ViewMatrix", GL_FALSE);
		shaderPrograms[CORE_PROGRAM]->setVec3f(cam.getPosition(), "cameraPosition" );
		light->sendUniforms(shaderPrograms[CORE_PROGRAM], 0);
		shaderPrograms[CORE_PROGRAM]->set1i(w.getLights().size(), "nLights");

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		w.render(shaderPrograms[CORE_PROGRAM]);
		sb.render( cam.getViewMatrix() );

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

	// Specify the number of samples to use for MSAA
	// glHint(GL_MULTISAMPLE_FILTER_HINT_NV, 4);

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

void processInput(std::unordered_set<std::string> input, WindowManager *window, ViewCamera *cam, float delta){

	// mouse

	WindowManager::BTN_STATE rightMouse = window->getMouseButtonState( WindowManager::MOUSE_BUTTON::RIGHT );

	if( rightMouse == WindowManager::BTN_STATE::PRESS ){
		double currMouseX, currMouseY;
		window->getCursorPos(&currMouseX, &currMouseY);
		
		if( lastStateRightBtn == WindowManager::BTN_STATE::PRESS)
			cam->rotate( -delta*glm::vec3( currMouseX - mouseX, mouseY - currMouseY, 0.f ) );
		
		mouseX = currMouseX;
		mouseY = currMouseY;
	}

	lastStateRightBtn = rightMouse;

	// keyboard

	if( input.find("ESCAPE") != input.end() )
		window->close();
	
	if( input.find("W") != input.end() )
		cam->move(glm::vec3(0.f, 0.f, delta));
	
	if( input.find("A") != input.end() )
		cam->move(glm::vec3(delta, 0.f, 0.f));
	
	if( input.find("S") != input.end() )
		cam->move(glm::vec3(0.f, 0.f, -delta));
	
	if( input.find("D") != input.end() )
		cam->move(glm::vec3(-delta, 0.f, 0.f));
	

}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	
	glViewport(0, 0, width, height);
	w.setAspectRatio((float)width, (float)height);

	for( ShaderProgram* sp : shaderPrograms )
		sp->setMat4fv(w.getPerspectiveMatrix(), "ProjectionMatrix", GL_FALSE);

}