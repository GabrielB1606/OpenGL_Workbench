#version 460 core

// in vec2 texCoord;
in vec3 fragPosition;
in vec3 normal;

uniform float eta;
uniform vec3 cameraPosition;
layout(binding = 5) uniform samplerCube SceneTexture;

out vec4 FragColor;

void main() {

	vec3 viewDir = normalize( fragPosition - cameraPosition );
	vec3 norm = normalize(normal);
	// vec3 objectNormal = normalize(vec3(TexCoord - 0.5, 0.0));
    vec3 refractDir = refract(viewDir, normal, eta);
    // vec3 refractDir = viewDir;

	FragColor = texture(SceneTexture, refractDir);

}