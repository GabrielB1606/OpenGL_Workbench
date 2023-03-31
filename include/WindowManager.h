#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "headers.h"
#include "OpenglLoader.h"

class WindowManager{

    private:
        GLFWwindow* window = nullptr;
        float deltaTime, time;

    public:
        enum BTN_STATE { PRESS = GLFW_PRESS, RELEASE = GLFW_RELEASE };
        enum MOUSE_BUTTON { LEFT = GLFW_MOUSE_BUTTON_LEFT, MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE, RIGHT = GLFW_MOUSE_BUTTON_RIGHT };

        WindowManager(int width, int height, std::string title, int GLMajVersion, int GLMinVersion);
        ~WindowManager();

        void processInput();
        bool isOpen();
        void getFramebufferSize(int *width, int *height);
        void swapBuffers();
        std::unordered_set<std::string> pollEvents();
        void close();

        BTN_STATE getMouseButtonState(MOUSE_BUTTON btn);
        void getCursorPos(double *x, double *y);

        float getDeltaTime();

        GLFWwindow* getWindow();

};

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

#endif