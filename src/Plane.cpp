#include "Plane.h"

Plane::Plane(int div, float width){

    float triangleSide = width/div;
    GLuint index;

    for(int row = 0; row <= div; row++)
        for(int col = 0; col <= div; col++){
            vertices.push_back( glm::vec3( col*triangleSide, 0.f, row*triangleSide ) );

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
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

    // link VAO
	glBindVertexArray(VAO);

    // bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
    // bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	
    // vertex attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
    // unbind all
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
}

void Plane::render(ShaderProgram *shader){

    material.sendUniforms(shader);
    shader->setMat4fv(glm::mat4(1.f), "ModelMatrix", false);

    shader->use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader->stopUsing();

}
