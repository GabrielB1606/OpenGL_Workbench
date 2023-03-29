#include "headers.h"
#include "World.h"

#include <noc_file_dialog.h>

const short glMajVersion = 4, glMinVersion = 6;

// Define the projection matrix
float fov = 90.0f;  // Field of view in degrees
float nearPlane = 0.1f;  // Near clipping plane
float farPlane = 1000.0f; // Far clipping plane

World w(fov, 1280, 720, nearPlane, farPlane);

WindowManager::BTN_STATE lastStateRightBtn = WindowManager::BTN_STATE::RELEASE;
double mouseX, mouseY;

void configOpenGL();
void processInput(std::unordered_set<std::string> input, WindowManager *window, ViewCamera *cam, float delta);

int main() {
	
	// const char *ret;
	// ret = noc_file_dialog_open(
	// 	NOC_FILE_DIALOG_OPEN,
	// 	"png\0*.png\0jpg\0*.jpg;*.jpeg\0",
	// 	std::filesystem::current_path().string().c_str(), NULL
	// );
	
	// std::cout << ret << "\n";
	// return 0;

	// create the window
	WindowManager windowManager(1280, 720, "window manager", glMajVersion, glMinVersion);
	// setup the graphics pipeline with OpenGL
	ShaderProgram s("460", glMajVersion, glMinVersion, "shaders/core/vertex.vert", "shaders/core/fragment.frag");
	// initial configuration
	configOpenGL();

	// setup the GUI
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	GraphicUserInterface gui(&windowManager, glMajVersion, glMinVersion);
	
	s.setMat4fv(w.getPerspectiveMatrix(), "ProjectionMatrix", GL_FALSE);
	w.loadMesh("models/Crate1.obj");

	// camera
	ViewCamera cam(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f) );

	// load a model
	BasicMesh* mesh = w.getMeshes()[0];
	mesh->translate( glm::vec3(0.f, 0.f, 5.f) );

	w.addLight( new Light(glm::vec3(0.f)) );
	Light *light = w.getLights()[0];

	float delta = 0.001f;

	std::unordered_set<std::string> input;

	windowManager.getDeltaTime();

	while ( windowManager.isOpen() ) {
		
		mesh->rotate( delta*glm::vec3(30.f, 30.f, 30.f) );

		input = windowManager.pollEvents();
		processInput(input, &windowManager, &cam, delta);

		s.setMat4fv(cam.getViewMatrix(), "ViewMatrix", GL_FALSE);
		s.setVec3f(cam.getPosition(), "cameraPosition" );
		light->sendUniforms(&s, 0);

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		w.render(&s);

		gui.draw();
		gui.render();

		windowManager.swapBuffers();
		delta = windowManager.getDeltaTime();

	}

	return 0;
}

void configOpenGL(){

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// Specify the depth function
	glDepthFunc(GL_LESS);
	
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
			cam->rotate( delta*glm::vec3( currMouseX - mouseX, mouseY - currMouseY, 0.f ) );
		
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
		cam->move(glm::vec3(-delta, 0.f, 0.f));
	
	if( input.find("S") != input.end() )
		cam->move(glm::vec3(0.f, 0.f, -delta));
	
	if( input.find("D") != input.end() )
		cam->move(glm::vec3(delta, 0.f, 0.f));
	

}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	
	glViewport(0, 0, width, height);
	w.setAspectRatio((float)width, (float)height);

}