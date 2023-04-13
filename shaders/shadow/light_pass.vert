#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

uniform mat4 ProjViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 InverseModelMatrix;

out vec2 texCoord;
out vec3 fragPosition;
out vec3 normal;

void main() {
	
	gl_Position = ProjViewMatrix * ModelMatrix * vec4(aPos.x , aPos.y, aPos.z, 1.0);
	
	texCoord = aTexCoords;
	fragPosition = (ModelMatrix * vec4(aPos, 1.0)).xyz;
	normal = normalize( vec4(transpose( InverseModelMatrix) * vec4(aNormal, 1.0)).xyz );

}