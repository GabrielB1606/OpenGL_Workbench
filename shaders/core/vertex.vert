#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main() {
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(aPos.x , aPos.y, aPos.z, 1.0);
	// gl_Position = ProjectionMatrix * ModelMatrix * vec4(aPos.x , aPos.y, aPos.z-1.0, 1.0);
	// gl_Position = ModelMatrix * vec4(aPos, 1.0);
}