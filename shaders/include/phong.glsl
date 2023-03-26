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
