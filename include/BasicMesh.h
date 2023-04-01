#ifndef BASICMESH_H
#define BASICMESH_H

#include "headers.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Material.h"
#include "BasicMeshEntry.h"
#include "BasicMesh.h"

class BasicMesh{
private:
    
    enum BUFFER_TYPE{
        INDEX_BUFFER = 0,
        POS_VB       = 1,
        TEXCOORD_VB  = 2,
        NORMAL_VB    = 3,
        WVP_MAT_VB   = 4,  // required only for instancing
        WORLD_MAT_VB = 5,  // required only for instancing
        NUM_BUFFERS  = 6
    };

    GLuint VAO;
    GLuint buffers[NUM_BUFFERS] = {0};

    std::vector<BasicMeshEntry> meshes;
    std::vector<Material> materials;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<GLuint> indices;

    Assimp::Importer importer;
    const aiScene* scene;

    // transformations
    glm::mat4 modelMatrix   = glm::mat4(1.f);
    glm::vec3 translation   = glm::vec3(0.f);
    glm::mat4 rotation      = glm::mat4(1.f);
    glm::vec3 scale         = glm::vec3(1.f);

public:
    BasicMesh();
    ~BasicMesh();
    void clear();
    
    // loading related functions
    bool loadMesh(std::string filename);
    bool initFromScene(const aiScene* scene, std::string filename);
    bool initSingleMesh(unsigned int meshIndex, const aiMesh* aiMeshPointer);
    bool initMaterials(const aiScene* scene, std::string textureDir);
    void loadColors(const aiMaterial *mat, int index);
    void loadTextures(std::string dir, const aiMaterial *mat, int index);
    void populateBuffers();
    
    // draw/render functions
    void render(ShaderProgram* shader);
    void sendUniforms(ShaderProgram* shader);

    // transformations functions
    void calculateModelMatrix();
    void translate(glm::vec3 vec);
    void rotate(glm::vec3 vec);
    void scaleUp(glm::vec3 vec);

    // getters
    glm::vec3* getTranslationReference();
    glm::vec3* getScaleReference();
};


#endif