#include "Plane.h"

Plane::Plane(int div, float width, glm::vec3 init_pos){

    reflection.init(2048, 2048);

    this->position.x = init_pos.x + (width/2);
    this->position.z = init_pos.z + (width/2);

    material.diffuseColor = glm::vec3(1.f);
    material.ambientColor = glm::vec3(1.f);
    material.specularColor = glm::vec3(1.f);

    material.textures[Texture::TYPE::DIFFUSE] = new Texture("models/tex512.png", GL_TEXTURE_2D);

    float triangleSide = width/div;
    GLuint index;

    for(int row = 0; row <= div; row++)
        for(int col = 0; col <= div; col++){

            vertices.push_back( init_pos + glm::vec3( triangleSide*col, 0.f, triangleSide*row ) );
            texCoords.push_back( glm::vec2( (float)(col%2), (float)(row%2) ) );
            normals.push_back( glm::vec3(0.f, 1.f, 0.f) );

            if( row<div && col<div ){
            
                index = row*(div+1) + col;

                // top triangle
                indices.push_back(index + 1);
                indices.push_back(index + div + 1);
                indices.push_back(index + div + 2);

                // bottom triangle
                indices.push_back(index);
                indices.push_back(index + (div+1) );
                indices.push_back(index + 1);

            }

        }

    // generate buffers
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &TBO);
	glGenBuffers(1, &NBO);

    // link VAO
	glBindVertexArray(VAO);

    // bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
 
    // bind TBO (?)
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // bind NBO (?)
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
    // unbind all
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
}

Plane::~Plane(){
}

void Plane::render(ShaderProgram *shader){

    material.sendUniforms(shader);

    reflection.bindRead(GL_TEXTURE0);
    shader->setMat4fv(glm::mat4(1.f), "ModelMatrix", false);

    shader->use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader->stopUsing();

}

void Plane::mirror(ShaderProgram *shader, std::vector<BasicMesh *> meshes, ViewCamera cam, glm::mat4 projectionMatrix){

    glClearStencil(0);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    this->render(shader);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glDisable(GL_DEPTH_TEST);

    // Normalize the normal vector
    glm::vec3 n = glm::normalize( glm::vec3(0.f, 1.f, 0.f) );

    // Compute the reflection matrix using the formula: I - 2 * (N * N^T)
    float nxn[16] = {
        n.x * n.x, n.x * n.y, n.x * n.z, 0.0f,
        n.x * n.y, n.y * n.y, n.y * n.z, 0.0f,
        n.x * n.z, n.y * n.z, n.z * n.z, 0.0f,
        0.0f,       0.0f,       0.0f,       1.0f
    };

    glm::mat4 reflectionMatrix = glm::mat4(1.0f) - 2.0f * glm::make_mat4(nxn);
    // Translate the reflection matrix by the position of the quad
    reflectionMatrix[3] = glm::vec4(-2.0f * position * n, 1.0f);


    glm::mat4 viewMatrix = reflectionMatrix * cam.getViewMatrix();
    glm::mat4 projViewMatrix = projectionMatrix * viewMatrix;
    shader->setMat4fv(projViewMatrix, "ProjViewMatrix", GL_FALSE);
    
    glEnable(GL_DEPTH_TEST);

    for(BasicMesh* mesh : meshes)
        mesh->render(shader);  

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);

}
