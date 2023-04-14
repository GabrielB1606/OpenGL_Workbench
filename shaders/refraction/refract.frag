#version 460 core

in vec2 texCoord;
in vec3 fragPosition;
in vec3 normal;

uniform vec3 cameraPosition;
layout(binding = 5) uniform sampler2D SceneTexture;

out vec4 FragColor;

void main() {

	FragColor = texture2D(SceneTexture, texCoord);

}