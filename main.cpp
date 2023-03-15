#include "headers.h"

const short glMajVersion = 4, glMinVersion = 6;

int main() {
	
	WindowManager windowManager(1280, 720, "window manager", glMajVersion, glMinVersion);

	// 1. Build and compile our shader programs
	ShaderProgram s("460", glMajVersion, glMinVersion, "shaders/core/vertex.vert", "shaders/core/fragment.frag");

	// Define the projection matrix
	float fov = 45.0f;  // Field of view in degrees
	float aspectRatio = 1280.0f / 720.0f; // Aspect ratio of the window
	float nearPlane = 0.1f;  // Near clipping plane
	float farPlane = 100.0f; // Far clipping plane

	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

	s.setMat4fv(projection, "projectionMatrix", false);

	glDisable(GL_CULL_FACE);
	// glEnable(GL_DEPTH_TEST);


	BasicMesh* mesh = new BasicMesh();
	mesh->loadMesh("models/triangle.obj");

	// 3. Set up vertex data and configure vertex attributes

	// Define three vertices with 3D positions
	// float vertices[] = {
	// 	-1.f, 0.0f, 0.0f,
	// 	0.0f, 1.0f, 0.0f,
	// 	1.0f,  0.0f, 0.0f
	// };

	// // Generate vertex buffer object (VBO) and vertex array object (VAO)
	// unsigned int VBO, VAO;
	// glGenVertexArrays(1, &VAO);
	// glGenBuffers(1, &VBO);

	// // Bind VAO, then bind VBO
	// glBindVertexArray(VAO);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// // Copy the vertex data into the buffer's memory
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// // Set attributes that describe how OpenGL should interpret the vertex data
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0);


	// // Unbind so other calls won't modify VBO and VAO
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	GraphicUserInterface gui(&windowManager, 3, 3);

	while ( windowManager.isOpen() ) {
		// processInput(window);
		windowManager.processInput();

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		// glUseProgram(shaderProgram);
		s.use();
		// glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		mesh->render();

		gui.draw();
		gui.render();

		windowManager.swapBuffers();
		windowManager.pollEvents();

		// s.stopUsing();

	}

	return 0;
}