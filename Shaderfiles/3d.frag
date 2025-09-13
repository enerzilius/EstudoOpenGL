#version 330 core
out vec4 FragColor;
  
in vec2 UV;
in float lightIntensity;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float mixParam; 

void main()
{
    //vec4 pixelColor = mix(texture(tex0, UV), texture(tex1, vec2(-UV.x, UV.y)), mixParam)*(1-UV.x) + ourColor;
    vec4 pixelColor = mix(texture(tex0, UV), texture(tex1, vec2(-UV.x, UV.y)), mixParam);
    float fade = smoothstep(0.0, 10.0, lightIntensity*10);
    FragColor = pixelColor * vec4(lightColor, 1.0)*fade;
}