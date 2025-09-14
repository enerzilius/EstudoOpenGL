#version 330 core
out vec4 FragColor;
  
in vec2 UV;

uniform vec3 objectColor;

void main()
{
    //vec4 pixelColor = mix(texture(tex0, UV), texture(tex1, vec2(-UV.x, UV.y)), mixParam)*(1-UV.x) + ourColor;
    vec4 pixelColor = vec4(objectColor, 1.0);
    FragColor = pixelColor;
}