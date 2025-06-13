#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 UV;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
    FragColor = mix(texture(tex0, UV), texture(tex1, UV), 0.2)*(1-UV.x);
}