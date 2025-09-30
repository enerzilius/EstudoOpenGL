#version 330 core
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;
uniform bool horizontal;
uniform float weight[5] = float[] ( 0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f );

void main()
{
	vec2 textureOffset = 1.0f / textureSize(screenTexture, 0);	
	vec3 result = texture(screenTexture, texCoords).rgb * weight[0];

	if(horizontal) {
		for(int i = 1; i < 5; i++){
			result += texture(screenTexture, texCoords + vec2(texCoords.x * i, 0.0)).rgb * weight[i];
			result += texture(screenTexture, texCoords - vec2(texCoords.x * i, 0.0)).rgb * weight[i];
		}
	}else {
		for(int i = 1; i < 5; i++){
			result += texture(screenTexture, texCoords + vec2(texCoords.y * i, 0.0)).rgb * weight[i];
			result += texture(screenTexture, texCoords - vec2(texCoords.y * i, 0.0)).rgb * weight[i];
		}
	}

	FragColor = vec4(result, 1.0);
}