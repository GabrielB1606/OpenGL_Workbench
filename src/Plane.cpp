#include "Plane.h"

Plane::Plane(int div, float width){

    float triangleSide = width/div;
    GLuint index;

    for(int row = 0; row <= div; row++)
        for(int col = 0; col <= div; col++){
            vertices.push_back( glm::vec3( col*triangleSide, 0.f, row*-triangleSide ) );

            if( row<div && col<div ){
            
                index = row*(div+1) + col;

                // top triangle
                indices.push_back(index);
                indices.push_back(index + (div+1) + 1);
                indices.push_back(index + (div+1));

                // bottom triangle
                indices.push_back(index);
                indices.push_back(index + 1);
                indices.push_back(index + (div+1) + 1);

            }

        }
        
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(), GL_STATIC_DRAW);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
    glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
}

void Plane::render(ShaderProgram *shader){

    material.sendUniforms(shader);

    shader->use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader->stopUsing();

}
