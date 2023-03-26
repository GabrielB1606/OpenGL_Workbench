#version 330 core

struct Colors{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 texCoord;
in vec3 vertexPosition;
in vec3 normal;

uniform Colors mat;
uniform vec3 cameraPosition;
uniform sampler2D DiffTexture;

uniform vec3 lightPosition;
uniform vec3 lightColor;

out vec4 FragColor;

vec3 calculateAmbient(vec3 ambient, vec3 lightColor, float intensity){
    return ambient * lightColor * intensity;
}

vec3 calculateDiffuse(vec3 diffuse, vec3 vertexPosition, vec3 normal, vec3 lightPosition){
    
    vec3 lightDir = normalize( lightPosition - vertexPosition );
    float factor = max( dot(lightDir, normalize(normal)), 0.0 );

    return factor * diffuse;
}

vec3 calculateSpecular(vec3 specular, vec3 vertexPosition, vec3 normal, vec3 lightPosition, vec3 camPosition){
    
    vec3 lightToPosNorm = normalize( vertexPosition - lightPosition );
    vec3 reflectNorm = normalize( reflect( lightToPosNorm, normalize(normal) ) );
    vec3 posToViewNorm = normalize( camPosition - vertexPosition );
    float specularConstant = pow( max( dot( posToViewNorm, reflectNorm ), 0 ), 35 );

    return specular * specularConstant;
}

void main() {

	vec3 ambientComponent = calculateAmbient( mat.ambient, lightColor, 0.2 );
	vec3 diffuseComponent = calculateDiffuse(mat.diffuse, vertexPosition, normal, lightPosition );
	vec3 specularComponent = calculateSpecular(mat.specular, vertexPosition, normal, lightPosition, cameraPosition );

	FragColor = texture2D(DiffTexture, texCoord) * vec4(ambientComponent + diffuseComponent + specularComponent, 1.0);
}