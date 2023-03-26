#version 330 core

// #extension GL_GOOGLE_include_directive : enable
// #include "../include/phong.glsl"


struct Colors{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
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
    float factor = max( dot(normalize(normal), lightDir), 0.0 );

    return factor * diffuse;
}

vec3 calculateSpecular(Colors mat, vec3 vertexPosition, vec3 normal, vec3 lightPosition, vec3 camPosition, vec3 lightColor){
    
    vec3 viewDirection = normalize( vertexPosition - camPosition );

    vec3 lightDirection = normalize( vertexPosition - lightPosition );
    vec3 reflectDirection = normalize( reflect( -lightDirection, normalize(normal) ) );

	float dotProduct = dot( viewDirection, reflectDirection );
	float specularConstant = 0.0;
	if( dotProduct > 0 && dot( viewDirection, lightDirection ) > 0 )
		specularConstant = pow( dotProduct, mat.shininess );

    return mat.specular * specularConstant * lightColor;
}

void main() {

	vec3 ambientComponent = calculateAmbient( mat.ambient, lightColor, 0.2 );
	vec3 diffuseComponent = calculateDiffuse(mat.diffuse, vertexPosition, normal, lightPosition );
	
	vec3 specularComponent = vec3(0.0);
	if( mat.shininess >= 0.0 )
		specularComponent =calculateSpecular(mat, vertexPosition, normal, lightPosition, cameraPosition, lightColor );

	FragColor = texture2D(DiffTexture, texCoord) * vec4(ambientComponent + diffuseComponent + specularComponent, 1.0);
}