#version 330 core
out vec4 FragColor;
  
in vec2 UV;
in vec3 Normal;
in vec3 WorldPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camPos;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float mixParam; 
uniform float shininess;

mat3 sobel_y = mat3(-1.0, -2.0, -1.0, 
                     0.0,  0.0,  0.0, 
                     1.0,  2.0,  1.0);

mat3 sobel_x = mat3(-1.0, 0.0, 1.0,
                    -2.0, 0.0, 2.0, 
                    -1.0, 0.0, 1.0);

void main()
{
    vec4 pixelColor = mix(texture(tex0, UV), texture(tex1, vec2(-UV.x, UV.y)), mixParam);
    //vec4 pixelColor = vec4(0.0, 0.0, 0.5, 1.0);

    // Blinn-Phong model
    float ambientStrength = 0.1;
    vec3 ambientColor = lightColor * ambientStrength;
    
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - WorldPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = diff * lightColor;

    vec3 viewDir = normalize(camPos - WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specularHighlight = lightColor * spec;
    
    FragColor = pixelColor * vec4(diffuseColor + ambientColor + specularHighlight, 1.0);
}