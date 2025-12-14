#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;
  
in vec2 UV;
in vec3 Normal;
in vec3 WorldPos;

uniform bool usesDiffuseMap;
uniform bool usesSpecularMap;
uniform bool usesGlowMap;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 camPos;

uniform sampler2D diffuseMap;
uniform sampler2D diffuseMap1;
uniform sampler2D diffuseMap2;
uniform sampler2D specularMap;
uniform sampler2D specularMap2;
uniform sampler2D glowMap;
uniform float mixParam; 
uniform float shininess;
uniform float ambientStrength;

struct DirLight {
    vec3 direction;
  
    vec3 lightColor;
};  
uniform DirLight dirLight;


struct PointLight {    
    vec3 position;
    
    //float constant;
    //float linear;
    //float quadratic;  

    float intensity;
    vec3 lightColor;
}; 
// passar o array inteiro ao invés disso aqui tavez?
#define NR_POINT_LIGHTS 3
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 ambColor, vec3 diffColor, vec3 specColor, vec3 glowColor) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    vec3 ambient  = ambientStrength * ambColor;
    vec3 diffuse  = diff * diffColor;
    vec3 specular = spec * specColor;
    return light.lightColor*(ambient + diffuse + specular + glowColor);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 ambColor, vec3 diffColor, vec3 specColor, vec3 glowColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    //float attenuation = 1.0 / (light.constant + light.linear * distance + 
  	//		     light.quadratic * (distance * distance));    
    float distanceFromLight = length(light.position - WorldPos);
    float attenuation = (1.0 / (1.0+distanceFromLight*distanceFromLight))*100 * light.intensity;

    // combine results
    vec3 ambient  = attenuation*(ambientStrength * ambColor);
    vec3 diffuse  = attenuation*(diff * diffColor);
    vec3 specular = attenuation*(spec * specColor);

    return light.lightColor*(ambient + diffuse + specular+glowColor);
}
void main()
{
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
    
    vec3 normal = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - WorldPos);
    vec3 viewDir = normalize(camPos - WorldPos);

    vec3 result = CalcDirLight(dirLight, normal, viewDir, _ambientColor, _diffuseColor, _specularColor, _glowColor);
    //vec3 result = vec3(dirLight.direction*-1);
    for(int i = 0; i < NR_POINT_LIGHTS; i++) result += CalcPointLight(pointLights[i], normal, WorldPos, viewDir, _ambientColor, _diffuseColor, _specularColor, _glowColor);

    FragColor = vec4(result, 1.0);

    float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 0.15f) BloomColor = FragColor;
    else BloomColor = vec4(0.0);
}


