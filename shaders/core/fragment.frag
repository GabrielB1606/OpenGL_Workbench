#version 330 core

in vec2 texCoord;

uniform sampler2D DiffTexture;

out vec4 FragColor;

void main() {
	FragColor = texture2D(DiffTexture, texCoord);
	// FragColor = vec4(1.0f, 0.0f, 0.2f, 1.0f);
}