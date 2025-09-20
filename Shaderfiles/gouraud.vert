#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
//layout (location = 2) in vec3 aNormal;
out vec2 UV;
out vec3 Normal;
out vec3 WorldPos;
out vec3 Light;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camPos;
uniform float shininess;

uniform mat4 model;   
uniform mat4 view; 
uniform mat4 proj; 

void main()
{
    mat4 MVP = proj * view * model;
    gl_Position =  MVP * vec4((aPos), 1.0);
    vec3 worldPos = (model * vec4(aPos, 1.0)).xyz;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    UV = aTexCoord;
    //Normal = aNormal;
    Normal = normalize(aPos);

    // Gouraud Shading

    float ambientStrength = 0.1;
    vec3 ambientColor = lightColor * ambientStrength;
    
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - WorldPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = diff * lightColor;

    vec3 viewDir = normalize(camPos - WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specularHighlight = lightColor * spec;

    Light = diffuseColor + ambientColor + specularHighlight;
}