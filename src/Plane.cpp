#include "Plane.h"

Plane::Plane(int div, float width, glm::vec3 init_pos){

    this->translation->x = init_pos.x + (width/2);
    this->translation->y = init_pos.y;
    this->translation->z = init_pos.z + (width/2);

    // Normalize the normal vector
    this->normal = glm::normalize( glm::vec3(0.f, 1.f, 0.f) );
    this->plane = { this->normal.x, this->normal.y, this->normal.z, -glm::dot(*this->translation, this->normal) };

    this->reflection = glm::mat4{
        1-2*plane.x*plane.x,  -2*plane.x*plane.y,  -2*plane.x*plane.z, -2*plane.x*plane.w,
         -2*plane.y*plane.x, 1-2*plane.y*plane.y,  -2*plane.y*plane.z, -2*plane.y*plane.w,
         -2*plane.z*plane.x,  -2*plane.z*plane.y, 1-2*plane.z*plane.z, -2*plane.z*plane.w,
                          0,                   0,                   0,                  1
    };

    this->reflection = glm::transpose(this->reflection);

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

    shader->setMat4fv(this->modelMatrix, "ModelMatrix", false);
    shader->setMat4fv(this->invModelMatrix, "InverseModelMatrix", false);

    shader->use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLsizei)0);
    glBindVertexArray(0);

    shader->stopUsing();

}

void Plane::mirror(ShaderProgram *shader, std::vector<BasicMesh *> meshes, ViewCamera *cam, glm::mat4 projectionMatrix, Skybox* sky){

    // glClearStencil(0);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    this->render(shader, projectionMatrix * cam->getViewMatrix());

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glDisable(GL_DEPTH_TEST);

    glm::mat4 projViewMatrix = projectionMatrix * cam->getViewMatrix() * this->reflection;


    glCullFace(GL_FRONT); // Invert front face culling

    if(sky != nullptr)
        sky->render(cam->getViewMatrix() * this->reflection);

    for(BasicMesh* mesh : meshes)
        mesh->render(shader, projViewMatrix);  
    
    glCullFace(GL_BACK); // Revert front face culling to its original state

    glEnable(GL_DEPTH_TEST);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
}

void Plane::render(ShaderProgram *shader, glm::mat4 projViewMatrix){

    material.sendUniforms(shader);

    shader->setMat4fv( this->modelMatrix , "ModelMatrix", false);
    shader->setMat4fv( this->invModelMatrix , "InverseModelMatrix", false);
    shader->setMat4fv(projViewMatrix * this->modelMatrix, "ProjViewModelMatrix", false);

    shader->use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLsizei)0);
    glBindVertexArray(0);

    shader->stopUsing();

}
