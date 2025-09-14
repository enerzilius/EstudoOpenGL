#version 330 core
out vec4 FragColor;
  
in vec2 UV;
in vec3 Normal;
in vec3 WorldPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float mixParam; 

void main()
{
    vec4 pixelColor = mix(texture(tex0, UV), texture(tex1, vec2(-UV.x, UV.y)), mixParam);
    //vec4 pixelColor = vec4(0.0, 0.0, 0.5, 1.0);

    float ambientStrength = 0.1;
    vec3 ambientColor = lightColor * ambientStrength;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - WorldPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = diff * lightColor;
    
    FragColor = pixelColor * vec4(diffuseColor + ambientColor, 1.0);
}