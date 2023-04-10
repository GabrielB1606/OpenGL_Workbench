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

void Plane::mirror(ShaderProgram *shader, std::vector<BasicMesh *> meshes, ViewCamera cam){

    // shader->setVec3f(*this->position, "lightPosition");

    reflection.bindWrite();

    glViewport(0, 0, 2048, 2048);

    // Compute the mirror plane by using the normal of the mirror quad
    glm::vec3 mirrorNormal = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec4 mirrorPlane = glm::vec4(mirrorNormal, 0.0f);


    // Compute the reflected camera position
    glm::vec4 reflectedCameraPosition = glm::reflect(glm::vec4(cam.getPosition(), 1.0f), mirrorPlane);

    // Compute the reflected camera target
    glm::vec4 reflectedCameraTarget = glm::reflect(glm::vec4(cam.getTarget(), 1.0f), mirrorPlane);

    // Compute the reflected camera up vector
    glm::vec4 reflectedCameraUp = glm::reflect(glm::vec4(cam.getUp(), 0.0f), mirrorPlane);


    // glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.f), 1.f, 0.01f, 1000.f);
    // glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.f), 1.f, 0.01f, 1000.f);
    glm::mat4 projectionMatrix = glm::ortho(-50.f, 50.0f, -50.0f, 50.0f, -1000.f, 100.f);
    shader->setMat4fv( projectionMatrix, "ProjectionMatrix", GL_FALSE );
    
    // glm::mat4 viewMatrix = glm::lookAt(this->position, position + glm::reflect(cam.getTarget(), mirrorNormal) , glm::vec3(0.f, 0.f, 1.f));
    // glm::mat4 viewMatrix = glm::lookAt(glm::vec3(reflectedCameraPosition), glm::vec3(reflectedCameraTarget), glm::vec3(reflectedCameraUp));
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(reflectedCameraPosition), glm::vec3(reflectedCameraTarget), -cam.getUp());
    // glm::mat4 viewMatrix = glm::lookAt( this->position, this->position + glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f) );
    // glm::mat4 viewMatrix = glm::lookAt( glm::vec3(0.f, -1.75f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f) );
    shader->setMat4fv(viewMatrix, "ViewMatrix", GL_FALSE);
    
    glm::mat4 projViewMatrix = projectionMatrix * viewMatrix;
    shader->setMat4fv(projViewMatrix, "ProjViewMatrix", GL_FALSE);

    // glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    for(BasicMesh* mesh : meshes)
        mesh->render(shader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);    

}
