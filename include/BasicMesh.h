#ifndef BASICMESH_H
#define BASICMESH_H

#include "headers.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Material.h"
#include "BasicMeshEntry.h"
#include "SceneFBO.h"
#include "CubeMapFBO.h"
#include "Skybox.h"
// #include "Plane.h"

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
    CubeMapFBO* surrounding = nullptr;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<GLuint> indices;

    Assimp::Importer importer;
    const aiScene* scene;
    std::string filename;

    // transformations
    glm::mat4 modelMatrix = glm::mat4(1.f);
    glm::mat4 invModelMatrix = glm::mat4(1.f);
    std::shared_ptr<glm::vec3> translation;
    std::shared_ptr<glm::mat4> rotation;
    std::shared_ptr<glm::vec3> scale;
    float eta = 0.5f;

    bool shadowCaster = false;
    bool shadowReceiver = true;
    bool refractive = false;

public:
    BasicMesh();
    ~BasicMesh();
    void clear();
    
    // loading related functions
    bool loadMesh(std::string filename);
    bool initFromScene(const aiScene* scene, std::string filename);
    bool initSingleMesh(size_t meshIndex, const aiMesh* aiMeshPointer);
    bool initMaterials(const aiScene* scene, std::string textureDir);
    void loadColors(const aiMaterial *mat, size_t index);
    void loadTextures(std::string dir, const aiMaterial *mat, size_t index);
    void populateBuffers();
    
    // draw/render functions
    void render(ShaderProgram* shader);
    void render(ShaderProgram* shader, glm::mat4 projViewMatrix);
    void renderRefractive(ShaderProgram* shader, glm::mat4 projViewMatrix, SceneFBO* sceneTexture);
    void renderRefractive(ShaderProgram* shader, glm::mat4 projViewMatrix);
    void sendUniforms(ShaderProgram* shader);
    void renderSurroundings(std::vector<BasicMesh*> meshes, Skybox* sky, ShaderProgram* shader);

    // transformations functions
    void calculateModelMatrix();
    void translate(glm::vec3 vec);
    void rotate(glm::vec3 vec);
    void scaleUp(glm::vec3 vec);

    glm::vec3 getPosition();
    glm::vec3 getScale();

    // getters
    std::shared_ptr<glm::vec3> getTranslationReference();
    std::shared_ptr<glm::vec3> getScaleReference();
    std::string getSubMeshName(size_t index);
    std::string getName();
    size_t getSubMeshesSize();
    bool isShadowCaster();
    bool isShadowReceiver();
    bool isRefractive();
    bool* getRefractiveReference();
    bool* getShadowCasterReference();
    bool* getShadowReceiverReference();
    float* getEtaReference();
    BasicMeshEntry getMeshEntry(size_t index);
    GLuint getVAO();
    void* getIndices_ptr(unsigned int offset = 0);

    // setters
    void setShadowCaster(bool b);
    void setShadowReceiver(bool b);
    void attatchPosition( std::shared_ptr<glm::vec3> position );
};


#endif