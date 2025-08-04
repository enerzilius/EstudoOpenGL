#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 UV;

uniform mat4 transform;

uniform mat4 model;   
uniform mat4 view; 
uniform mat4 proj; 

void main()
{
    mat4 MVP = proj * view * model;
    gl_Position =  transform * MVP * vec4((aPos), 1.0);
    ourColor = vec4(aColor, 1.0);
    UV = aTexCoord;
}