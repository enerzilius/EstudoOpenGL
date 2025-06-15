#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 UV;

uniform float mixParam; 
uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
    FragColor = mix(texture(tex0, UV), texture(tex1, vec2(-UV.x, UV.y)), mixParam)*(1-UV.x);
    //vec4 pixelColor = texture(tex1, vec2(UV.x, UV.y));
    //if(pixelColor.a == 0) pixelColor.rgba = vec4(0.3, 0.0, 0.3, 1.0);
    //FragColor = pixelColor;
}