#version 330 core
out vec4 FragColor;
  
in vec2 UV;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float mixParam; 

void main()
{
    //vec4 pixelColor = mix(texture(tex0, UV), texture(tex1, vec2(-UV.x, UV.y)), mixParam)*(1-UV.x) + ourColor;
    vec4 pixelColor = mix(texture(tex0, UV), texture(tex1, vec2(-UV.x, UV.y)), mixParam)*(1-UV.x);
    FragColor = pixelColor;
}