#include "headers.h"

const short glMajVersion = 4, glMinVersion = 6;

int main() {
	
	WindowManager windowManager(1280, 720, "window manager", glMajVersion, glMinVersion);

	// 1. Build and compile our shader programs
	ShaderProgram s("460", glMajVersion, glMinVersion, "shaders/core/vertex.vert", "shaders/core/fragment.frag");

	// Define the projection matrix
	float fov = 90.0f;  // Field of view in degrees
	float aspectRatio = 1280.0f / 720.0f; // Aspect ratio of the window
	float nearPlane = 0.1f;  // Near clipping plane
	float farPlane = 1000.0f; // Far clipping plane

	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

	s.setMat4fv(projection, "projectionMatrix", false);

	glDisable(GL_CULL_FACE);
	// glEnable(GL_DEPTH_TEST);

	BasicMesh* mesh = new BasicMesh();
	mesh->loadMesh("models/triangle.obj");

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	GraphicUserInterface gui(&windowManager, 3, 3);

	mesh->translate( glm::vec3(0.f, -1.0f, 0.0f) );

	while ( windowManager.isOpen() ) {
		// processInput(window);
		windowManager.processInput();

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		// glUseProgram(shaderProgram);

		mesh->sendUniforms(&s);
		s.use();
		mesh->render();

		gui.draw();
		gui.render();

		windowManager.swapBuffers();
		windowManager.pollEvents();

		// s.stopUsing();

	}

	return 0;
}