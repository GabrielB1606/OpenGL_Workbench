#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "headers.h"

class WindowManager{

    private:
        GLFWwindow* window = nullptr;

    public:
        WindowManager(int width, int height, std::string title, int GLMajVersion, int GLMinVersion);
        ~WindowManager();

        void processInput();
        bool isOpen();
        void getFramebufferSize(int *width, int *height);
        void swapBuffers();
        std::unordered_set<std::string> pollEvents();

        GLFWwindow* getWindow();

};

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

#endif