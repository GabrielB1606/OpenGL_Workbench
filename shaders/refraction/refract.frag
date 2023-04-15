#version 460 core

in vec2 texCoord;
in vec3 fragPosition;
in vec3 normal;

uniform float eta;
layout(binding = 5) uniform sampler2D SceneTexture;

out vec4 FragColor;

void main() {

	vec3 viewDir = normalize(vec3(0.0, 0.0, 1.0));
	// vec3 objectNormal = normalize(vec3(TexCoord - 0.5, 0.0));
    vec3 refractDir = refract(viewDir, normal, eta);
	vec2 newTexCoord = texCoord + refractDir.xy;


	FragColor = texture2D(SceneTexture, newTexCoord);

}