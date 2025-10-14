#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;
  
in vec2 UV;
in vec3 Normal;
in vec3 WorldPos;

uniform bool usesDiffuseMap;
uniform bool usesSpecularMap;
uniform bool usesGlowMap;
uniform vec3 lightPos;
uniform vec3 lightDirection;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 camPos;
uniform vec3 lightColor;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D glowMap;
uniform float mixParam; 
uniform float shininess;
uniform float cutoffAngle;

void main()
{
    vec3 lightDir = normalize(lightPos - WorldPos);
    float theta = dot(lightDir, normalize(-lightDirection));
    vec3 _ambientColor = ambientColor;
    vec3 _diffuseColor = diffuseColor;
    vec3 _specularColor = specularColor;
    vec3 _glowColor = vec3(0.0);
    if(usesDiffuseMap) {
        _ambientColor = texture(diffuseMap, UV).xyz;
        _diffuseColor = texture(diffuseMap, UV).xyz;
    }
    if(usesSpecularMap) _specularColor = texture(specularMap, UV).xyz;
    if(usesGlowMap) _glowColor = texture(glowMap, UV).xyz;
    if(theta > cutoffAngle) {
        
        //vec4 pixelColor = vec4(0.0, 0.0, 0.5, 1.0);

        // Blinn-Phong model
        float ambientStrength = 0.1;
        vec3 ambient = _ambientColor * ambientStrength;
    
        vec3 normal = normalize(Normal);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = _diffuseColor * diff;

        //Blinn-Phong highlights
        vec3 viewDir = normalize(camPos - WorldPos);
        //vec3 halfwayDir = normalize(lightDir + viewDir);
        //float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
        //vec3 specularHighlight = _specularColor * spec;

        //Phong
        // viewDir
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        float specularStrength = 1.0;
        vec3 specularHighlight = _specularColor * spec * specularStrength;

        float distan = length(lightPos - WorldPos);
        float attenuation = (1.0 / (1.0+distan*distan))*100;

        ambient  *= attenuation; 
        diffuse  *= attenuation;
        specularHighlight *= attenuation; 
    
        FragColor = vec4(lightColor,1.0) * vec4(diffuse + ambient + specularHighlight + _glowColor, 1.0);

        float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
        if(brightness > 0.15f) BloomColor = FragColor;
        else BloomColor = vec4(0.0);
    }  else FragColor = vec4(_diffuseColor*0.1 + _glowColor, 1.0) ;
}