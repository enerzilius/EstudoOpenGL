#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 UV;

uniform float mixParam; 
uniform sampler2D tex0;
uniform sampler2D tex1;

vec3 map(float val){
    if(val < 0.03) return vec3(1.0, 1.0, 1.0);
    if(val < 0.1) return vec3(0.95, 0.0, 0.95);
    if(val < 1.0) return vec3(0.4, 0.4, 0.99);
    return vec3(val);
}

void main()
{
    vec4 color;
    float med =(UV.x+UV.y)/2;
    float val_x = pow(UV.x-med, 2);
    float val_y = pow(UV.y-med, 2);
    float s = val_x+val_y;
    vec3 rg = map(s);
    color = vec4(rg, 1);
    if((UV.x <= 0.3 && UV.x >= -0.3) || (UV.y <= 0.3 && UV.y >= -0.3)) color = vec4(0.0, 0.0, 0.0, 0.0);
    FragColor = color;
}