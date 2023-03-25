#version 330 core

struct Colors{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 texCoord;

uniform Colors mat;
uniform sampler2D DiffTexture;

out vec4 FragColor;

void main() {
	FragColor = texture2D(DiffTexture, texCoord);
	// FragColor = vec4(mat.specular, 1.0);
}