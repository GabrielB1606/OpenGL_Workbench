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
#include "Plane.h"
#include "SceneFBO.h"

#include <stb_particle_system/stb_particle_system.h>

// GLSL Version
const short glMajVersion = 4, glMinVersion = 6;
std::string glVersion_str = std::to_string(glMajVersion) + std::to_string(glMinVersion) + "0";

const int initial_width = 1280, inital_height = 720;

// Define the current state of the 3D world
World w(90.f, initial_width, inital_height, 0.1f, 1000.f);

// Manage window creation
WindowManager windowManager(initial_width, inital_height, "window manager", glMajVersion, glMinVersion);

// All shader programs used in the application
std::array<ShaderProgram*, 8> shaderPrograms = {
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/core/vertex.vert", "shaders/core/fragment.frag"),
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/skybox/skybox.vert", "shaders/skybox/skybox.frag"),
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/shadow/shadow_pass.vert", "shaders/shadow/shadow_pass.frag"),
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/shadow/light_pass.vert", "shaders/shadow/light_pass.frag"),
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/plain/vertex.vert", "shaders/plain/fragment.frag"),
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/reflect/reflect.vert", "shaders/reflect/reflect.frag"),
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/refraction/refract.vert", "shaders/refraction/refract.frag"),
	new ShaderProgram(glVersion_str.c_str(), glMajVersion, glMinVersion, "shaders/particle/sample.vert", "shaders/particle/sample.frag")
};

// camera
ViewCamera mainCamera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f) );

// Input Processor
InputProcessor input(&windowManager);

// setup the GUI
GraphicUserInterface gui(&windowManager, glMajVersion, glMinVersion);

void configOpenGL();

int main() {
	
	// initial configuration
	configOpenGL();

	// background color
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	// add default light
	w.addLight( new Light(glm::vec3(0.f)) );
	// w.getLight(0)->loadMesh("models\\estrellica.obj");
	
	// load a model
	// w.loadMesh("models\\Crate1.obj");
	// w.loadMesh("models\\estrellica.obj");
	
	// // translate models
	// w.getMeshes()[0]->translate(glm::vec3(0.f, 0.f, 7.f) );
	// w.getMeshes()[1]->translate(glm::vec3(1.f, 1.f, 2.5f));
	// w.getMeshes()[1]->scaleUp(-glm::vec3(0.975f, 0.975f, 0.975f));
	// w.getMeshes()[1]->rotate(glm::vec3(90.f, 0.f, 0.f));
	// w.getMeshes()[1]->attatchPosition( w.getLight(0)->getPositionReference() );
	

	// time between frames
	float delta = 0.001f;

	// skybox setup
	w.createSkybox(shaderPrograms[SKYBOX_PROGRAM], "shaders/skybox/sky/", "jpg");
	shaderPrograms[SKYBOX_PROGRAM]->setMat4fv(w.getPerspectiveMatrix(), "ProjectionMatrix", GL_FALSE);

	// send uniforms
	mainCamera.sendUniforms( shaderPrograms[CORE_PROGRAM] );
	mainCamera.sendUniforms( shaderPrograms[LIGHT_PASS] );

	w.createFloor();

	// start counting time between frames
	windowManager.getDeltaTime();

	// std::shared_ptr<SceneFBO> sceneFBO;

	ParticleProps p;
	p.color_begin = glm::vec4(1.f,0.f,0.f,1.f);
	p.color_end = glm::vec4(1.f,0.f,0.f,1.f);
	p.life_time = 30000.f;
	p.position = glm::vec3(0.f, 0.f, 2.f);
	p.velocity = glm::vec3(0.f);
	p.velocity_variation = glm::vec3(0.f);
	p.size_begin = 30.f;
	p.size_end = 30.f;
	p.size_variation = 1.f;

	ParticleSystem ps;


	// PARTICLE SANDBOX

	unsigned int quad_VA = 0, quad_VB = 0, quad_EB = 0;
    
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};  

	glGenVertexArrays(1, &quad_VA);
    glGenBuffers(1, &quad_VB);
    glGenBuffers(1, &quad_EB);

    glBindVertexArray(quad_VA);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, quad_VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_EB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);

	// unbind all
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	unsigned int transform_uniform_loc, color_uniform_loc, projview_uniform_loc;


	// for (size_t i = 0; i < 200; i++)
	// 	ps.Emit(p);
	
	

	while ( windowManager.isOpen() ) {
		
		// rotate just bc
		// w.getMeshes()[0]->rotate( delta*glm::vec3(30.f, 30.f, 30.f) );
		// w.getMeshes()[1]->rotate(delta*glm::vec3(0.f, 90.f, 0.f));

		// Only if there's camera movement, send the view matrix again
		if(input.process(&mainCamera, delta)){
			mainCamera.sendUniforms( shaderPrograms[CORE_PROGRAM] );
			mainCamera.sendUniforms( shaderPrograms[LIGHT_PASS] );
			mainCamera.sendUniforms( shaderPrograms[RENDER_REFRACT] );
		}

		// these are pretty much light uniforms
		w.sendUniforms( shaderPrograms[CORE_PROGRAM]);
		w.sendUniforms( shaderPrograms[LIGHT_PASS] );

		// render shadow cubemap
		w.renderShadowCubeMaps(shaderPrograms[SHADOW_PASS]);

		w.refreshRefractiveSurroundings(shaderPrograms[LIGHT_PASS]);

		// clear main framebuffer
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glViewport(0, 0, (int)w.getWidth(), (int)w.getHeight());

		// render the scene from the reflective surface
		// w.renderFloor(shaderPrograms[LIGHT_PASS], &mainCamera);
		
		// render the normal scene
		// render meshes
		w.renderMeshes(shaderPrograms[LIGHT_PASS], &mainCamera);

		// sceneFBO = w.renderSceneFBO(shaderPrograms[LIGHT_PASS], &mainCamera);

		w.renderRefractions(shaderPrograms[RENDER_REFRACT], &mainCamera);

		// sceneFBO.reset();
		
		// render skybox
		// w.renderSkybox( mainCamera.getViewMatrix() );

		// PARTICLE SANDBOX UPDATE/RENDER
		glDisable(GL_CULL_FACE);
		shaderPrograms[PLAIN_PROGRAM]->use();
		unsigned int shader_id = shaderPrograms[PLAIN_PROGRAM]->getID();

		projview_uniform_loc = glGetUniformLocation(shader_id, "u_ProjView");
		transform_uniform_loc = glGetUniformLocation(shader_id, "u_Transform");
		color_uniform_loc = glGetUniformLocation(shader_id, "u_Color");

		glm::mat4 projection_view_matrix = w.getPerspectiveMatrix()*mainCamera.getViewMatrix();
		// glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 3.f));
		glm::mat4 transform = glm::mat4(1.f);

		shaderPrograms[PLAIN_PROGRAM]->setMat4fv(projection_view_matrix, "u_ProjView", GL_FALSE);
		shaderPrograms[PLAIN_PROGRAM]->setMat4fv(transform, "u_Transform", GL_FALSE);
		// glUniformMatrix4fv(projview_uniform_loc, 1, GL_FALSE, glm::value_ptr(projection_view_matrix));
		// glUniformMatrix4fv(transform_uniform_loc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(quad_VA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		shaderPrograms[PLAIN_PROGRAM]->stopUsing();
		// ps.onUpdate( delta );
		// shaderPrograms[PLAIN_PROGRAM]->stopUsing();
		// shaderPrograms[PLAIN_PROGRAM]->use();
		// ps.onRender(shaderPrograms[PLAIN_PROGRAM]->getID(), w.getPerspectiveMatrix()*mainCamera.getViewMatrix() );
		// shaderPrograms[PLAIN_PROGRAM]->stopUsing();

		// render GUI
		gui.draw(&w, &mainCamera, &input, &clear_color);
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

	// mirrors
	glEnable(GL_STENCIL_TEST);
	// glEnable(GL_CLIP_DISTANCE0);

}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	
	glViewport(0, 0, width, height);
	w.setAspectRatio((float)width, (float)height);

}