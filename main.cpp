#include "headers.h"

const short glMajVersion = 4, glMinVersion = 6;

glm::mat4 projection = glm::mat4(1.f);

// Define the projection matrix
float fov = 90.0f;  // Field of view in degrees
float aspectRatio = 1280.0f / 720.0f; // Aspect ratio of the window
float nearPlane = 1.f;  // Near clipping plane
float farPlane = 99.0f; // Far clipping plane

int main() {
	
	WindowManager windowManager(1280, 720, "window manager", glMajVersion, glMinVersion);

	// 1. Build and compile our shader programs
	ShaderProgram s("460", glMajVersion, glMinVersion, "shaders/core/vertex.vert", "shaders/core/fragment.frag");


	projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

	s.setMat4fv(projection, "ProjectionMatrix", GL_FALSE);

	glDisable(GL_CULL_FACE);
	// glEnable(GL_CULL_FACE);
	// glFrontFace(GL_CW);
	// glCullFace(GL_BACK);
	// glEnable(GL_DEPTH_TEST);

	BasicMesh* mesh = new BasicMesh();
	mesh->loadMesh("models/triangle.obj");

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	GraphicUserInterface gui(&windowManager, glMajVersion, glMinVersion);

	mesh->translate( glm::vec3(0.f, 0.f, -5.0f) );
	// mesh->rotate( glm::vec3(0.f, 0.f, 30.f) );
	// mesh->scaleUp( glm::vec3(-0.5f, 0.f, 0.f) );

	while ( windowManager.isOpen() ) {
		
		mesh->rotate( glm::vec3(0.f, 0.f, 30.f*0.001f) );
		// mesh->rotate( glm::vec3(0.f, 30.f*0.001f, 0.f) );

		windowManager.processInput();

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		mesh->render(&s);

		gui.draw();
		gui.render();

		windowManager.swapBuffers();
		windowManager.pollEvents();

	}

	return 0;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	
	glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(fov), (float)width/(float)height, nearPlane, farPlane);

}