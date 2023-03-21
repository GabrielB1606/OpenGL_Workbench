#include "headers.h"

const short glMajVersion = 4, glMinVersion = 6;

glm::mat4 projection = glm::mat4(1.f);

// Define the projection matrix
float fov = 90.0f;  // Field of view in degrees
float aspectRatio = 1280.0f / 720.0f; // Aspect ratio of the window
float nearPlane = 0.1f;  // Near clipping plane
float farPlane = 1000.0f; // Far clipping plane

void configOpenGL();

int main() {
	
	// create the window
	WindowManager windowManager(1280, 720, "window manager", glMajVersion, glMinVersion);
	// setup the graphics pipeline with OpenGL
	ShaderProgram s("460", glMajVersion, glMinVersion, "shaders/core/vertex.vert", "shaders/core/fragment.frag");
	// initial configuration
	configOpenGL();

	// setup the GUI
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	GraphicUserInterface gui(&windowManager, glMajVersion, glMinVersion);
	
	// initial projection matrix
	projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	s.setMat4fv(projection, "ProjectionMatrix", GL_FALSE);	

	// camera
	glm::vec3 	cameraPos = glm::vec3(0.f, 0.f, 1.f),
				U = glm::vec3(1.f, 0.f, 0.f),
				V = glm::vec3(0.f, 1.f, 0.f),
				N = glm::vec3(0.f, 0.f, 1.f);

	glm::mat4 viewMatrix(
		U.x, U.y, U.z, -cameraPos.x,
		V.x, V.y, V.z, -cameraPos.y,
		N.x, N.y, N.z, -cameraPos.z,
		0.f, 0.f, 0.f, 1.f
	);
	// glm::mat4 lookMatrix = glm::lookAt(cameraPos, cameraPos+N, V);
	s.setMat4fv(viewMatrix, "ViewMatrix", GL_FALSE);
	

	// load a model
	BasicMesh* mesh = new BasicMesh();
	mesh->loadMesh("models/Crate1.obj");
	mesh->translate( glm::vec3(0.f, 0.f, -5.f) );

	float delta = 0.001f;

	while ( windowManager.isOpen() ) {
		
		mesh->rotate( glm::vec3(0.f, 0.f, 30.f*delta) );

		windowManager.processInput();

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		mesh->render(&s);

		gui.draw();
		gui.render();

		windowManager.swapBuffers();
		windowManager.pollEvents();

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
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	
	glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(fov), (float)width/(float)height, nearPlane, farPlane);

}