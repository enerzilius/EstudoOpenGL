#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
void main(){
	FragColor = vec4(vertexColor.x, 0, vertexColor.y, 1.0);
}