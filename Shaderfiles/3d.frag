#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;
  
in vec2 UV;
in vec3 Normal;
in vec3 WorldPos;

uniform bool usesTexture;
uniform vec3 lightPos;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 camPos;

uniform sampler2D tex0;
uniform float mixParam; 
uniform float shininess;

void main()
{
    vec4 textureColor = vec4(1.0);
    if(usesTexture) textureColor = texture(tex0, UV);
    //vec4 pixelColor = vec4(0.0, 0.0, 0.5, 1.0);

    // Blinn-Phong model
    float ambientStrength = 0.1;
    vec3 ambient = ambientColor * ambientStrength;
    
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - WorldPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseColor * diff;

    vec3 viewDir = normalize(camPos - WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specularHighlight = specularColor * spec;
    
    FragColor = textureColor * vec4(diffuse + ambient + specularHighlight, 1.0);

    float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 0.15f) BloomColor = FragColor;
    else BloomColor = vec4(0.0);
}