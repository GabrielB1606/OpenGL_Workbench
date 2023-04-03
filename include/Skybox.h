#ifndef SKYBOX_H
#define SKYBOX_H

#include "headers.h"

#include "ShaderProgram.h"

class Skybox{
private:
    
    ShaderProgram* skyboxProgram;
    std::string facesCubemap[6];

    unsigned int cubemapTexture;

    unsigned int VAO, VBO, EBO;

    float skyboxVertices[24] = {
            //   Coordinates
           -1.0f, -1.0f,  1.0f,//        7--------6
            1.0f, -1.0f,  1.0f,//       /|       /|
            1.0f, -1.0f, -1.0f,//      4--------5 |
           -1.0f, -1.0f, -1.0f,//      | |      | |
           -1.0f,  1.0f,  1.0f,//      | 3------|-2
            1.0f,  1.0f,  1.0f,//      |/       |/
            1.0f,  1.0f, -1.0f,//      0--------1
           -1.0f,  1.0f, -1.0f
        };

    unsigned int skyboxIndices[36] = {
        // Right
        1, 6, 2,
        6, 1, 5,
        // Left
        0, 7, 4,
        7, 0, 3,
        // Top
        4, 6, 5,
        6, 4, 7,
        // Bottom
        0, 2, 3,
        2, 0, 1,
        // Back
        0, 5, 1,
        5, 0, 4,
        // Front
        3, 6, 7,
        6, 3, 2
    };

public:
    Skybox(ShaderProgram* skyboxProgram, std::string directory, std::string format);
    virtual ~Skybox(){}

    void initCubeModel();
    void loadCubeTextures(std::string directory, std::string format = "jpg");

    void render(glm::mat4 viewMatrix);
};

#endif
