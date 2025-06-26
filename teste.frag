#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 UV;

uniform float mixParam; 
uniform sampler2D tex0;
uniform sampler2D tex1;

vec2 map(vec2 val){
    if(val.x < 0.1 && val.y < 0.1){
        return vec2(0.0, 0.0);
    }
    if(val.x > 0.25 && val.y < 0.2){
        return vec2(0.6, 0.1);
    }
    if((val.x < 0.25 && val.x > 0.1) && val.y < 0.2){
        return vec2(0.2, 0.0);
    }
    return val;
}

void main()
{
    vec4 color;
    float med =(UV.x+UV.y)/2;
    float val_x = UV.x-med;
    float val_y = UV.y-med;
    vec2 rg = vec2(val_x, val_y);
    rg = map(rg);
    color = vec4(rg.x, rg.y, 0, 1);
    FragColor = color;
}