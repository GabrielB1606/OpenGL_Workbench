#ifndef OPENGLLOADER_H
#define OPENGLLOADER_H

#include "headers.h"

class OpenglLoader{
    private:
        OpenglLoader(/* args */) {}
    
    public:
        
        static int load(){
            // glad init
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cout << "Failed to initialize GLAD" << std::endl;
                return -1;
            }
            return 0;
        }

};

#endif