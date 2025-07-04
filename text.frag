#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 UV;

uniform float mixParam; 
uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
    vec4 color;
    if(UV.y >= 0.67) color = vec4(0.0, 0.0, 0.0, 1.0);
    if(UV.y < 0.67 && UV.y > 0.34) color = vec4(0.5, 0.0, 0.0, 1.0);
    if(UV.y <= 0.34) color = vec4(0.5, 0.5, 0.0, 1.0);
    vec4 pixelColor = mix(texture(tex0, UV), texture(tex1, vec2(-UV.x, UV.y)), mixParam)*(1-UV.x) + ourColor;
    vec4 faceColor = texture(tex1, vec2(UV.x, UV.y))*(1-UV.x);
    //if(faceColor.a == 0) pixelColor.rgba = vec4(0.3f, 0.0f, 0.2f, 0.0f
    FragColor = pixelColor;
}