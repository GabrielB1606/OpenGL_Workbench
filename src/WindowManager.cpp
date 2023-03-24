#include "WindowManager.h"

WindowManager::WindowManager(int width, int height, std::string title, int GLMajVersion, int GLMinVersion){
 
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLMajVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLMinVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
    this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if(this->window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
        this->window = nullptr;
        return;
	}

	glfwMakeContextCurrent(this->window);

    if( OpenglLoader::load() != 0  ){
        glfwTerminate();
        return;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(this->window, frameBufferSizeCallback);
}

WindowManager::~WindowManager(){
    glfwTerminate();
}

GLFWwindow* WindowManager::getWindow(){
    return this->window;
}

void WindowManager::processInput() {
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->window, true);
}

bool WindowManager::isOpen(){
    return !glfwWindowShouldClose(window);
}

void WindowManager::getFramebufferSize(int *width, int *height){
    glfwGetFramebufferSize(this->window, width, height);
}

void WindowManager::swapBuffers(){
    glfwSwapBuffers(this->window);
}

std::unordered_set<std::string> WindowManager::pollEvents(){
    glfwPollEvents();

    std::unordered_set<std::string> input;

    if(glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        input.insert("ESCAPE");

    if( glfwGetKey(this->window, GLFW_KEY_W ) == GLFW_PRESS )
        input.insert("W");
    
    if( glfwGetKey(this->window, GLFW_KEY_A ) == GLFW_PRESS )
        input.insert("A");
    
    if( glfwGetKey(this->window, GLFW_KEY_S ) == GLFW_PRESS )
        input.insert("S");
    
    if( glfwGetKey(this->window, GLFW_KEY_D ) == GLFW_PRESS )
        input.insert("D");

    return input;

}

void WindowManager::close(){
    glfwSetWindowShouldClose(this->window, true);
}

WindowManager::BTN_STATE WindowManager::getMouseButtonState(WindowManager::MOUSE_BUTTON btn){
    return glfwGetMouseButton(this->window, btn) == GLFW_PRESS? WindowManager::BTN_STATE::PRESS : WindowManager::BTN_STATE::RELEASE;
}

void WindowManager::getCursorPos(double *x, double *y){

    glfwGetCursorPos(this->window, x, y);

}

float WindowManager::getDeltaTime(){
    
    float timeNow = (float)glfwGetTime();
    this->deltaTime = timeNow - time;
    time = timeNow;

    return this->deltaTime;
}
