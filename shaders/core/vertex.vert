#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec2 texCoord;
out vec3 vertexPosition;
out vec3 normal;

void main() {
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(aPos.x , aPos.y, aPos.z, 1.0);
	texCoord = aTexCoords;
	vertexPosition = (ModelMatrix * vec4(aPos, 1.0)).xyz;
	// normal = normalize( vec4(transpose( inverse(ModelMatrix) ) * vec4(aNormal, 1.0)).xyz );
	normal = normalize(ModelMatrix * vec4(aNormal, 1.0)).xyz;
}