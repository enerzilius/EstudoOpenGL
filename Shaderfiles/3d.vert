#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 UV;
out vec3 Normal;
out vec3 WorldPos;

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
    Normal = normalize(aPos);
}