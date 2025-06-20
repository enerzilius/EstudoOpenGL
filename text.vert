#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 UV;

uniform vec3 offset;

void main()
{
    gl_Position = vec4((aPos+offset)*(2*offset), 1.0);
    ourColor = vec4(aColor+offset, 1.0);
    UV = aTexCoord;
}