#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 UV;

uniform float mixParam; 
uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
    vec4 pixelColor = mix(texture(tex0, UV), texture(tex1, vec2(-UV.x, UV.y)), mixParam)*(1-UV.x) + ourColor;
    vec4 faceColor = texture(tex1, vec2(UV.x, UV.y))*(1-UV.x);
    FragColor = pixelColor;
}