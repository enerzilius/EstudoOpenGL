#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void resize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void render(GLFWwindow* window);
void colorLooping(GLuint program);

//coordenadas dos vértices do triângulo
	//z = 0 para fazer uma imagem 2d
float vertices[] = {
	 -1.0f,  0.5f, 0.0f, // esquerda cima
	 -0.5f, -0.5f, 0.0f, // meio do negativo
	  0.0f,  0.5f, 0.0f, // meio cima
	  //segundo e terceiro
	  0.5f, -0.5f, 0.0f,
	  1.0f,  0.5f, 0.0f,
	  // triangulos pra baixo
	  0.0f, -0.5f, 0.0f,
	  0.0f, -1.0f, 0.0f,
	  //topo
	  0.0f,  1.0f, 0.0f
};

GLuint indices[] = {
	0, 1, 2,
	2, 3, 4,
	2, 1, 3,
	2, 3, 6,
	1, 5, 6,
	0, 7, 4
};

float triangle[] = {
	// positions         // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // top 
	-1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, //canto superior esquerdo
	 1.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, //canto superior direito
};

GLuint triangleIndices[] = {
	0, 1, 2,
};


int main() {
	int vertexCount = sizeof(indices) / sizeof(GLuint)/2;
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Omor", NULL, NULL);

	glfwSetFramebufferSizeCallback(window, resize);

	if (window == NULL) {
		std::cout << "Falha na criação da janela" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	glfwSetFramebufferSizeCallback(window, resize);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader shaderProgram("offset.vert", "default.frag");

	VAO VAO1; 
	VAO1.Bind();

	VBO VBO1(triangle, sizeof(triangle));
	EBO EBO1(triangleIndices, sizeof(triangleIndices));

	VAO1.LinkVBO(VBO1, 0, 6, 0);
	VAO1.LinkVBO(VBO1, 1, 6, 3);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//loop de renderização
	while (!glfwWindowShouldClose(window))
	{
		//inputs
		processInput(window);

		//aqui vai os processos de renderização
		glClearColor(0.3f, 0.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();
		VAO1.Bind();
		float offset = 0.5f;
		shaderProgram.setFloat("xOffset", offset);

		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);

		//checar por eventos e trocar os buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void render(GLFWwindow* window) {

	// here we put our rendering code

}

void colorLooping(GLuint program) {
	GLfloat timeValue = 0.3 * glfwGetTime();
	GLfloat red = (sin(timeValue + 1) / 2.0f) + 0.5f;
	GLfloat green = (sin(timeValue) / 2.0f) + 0.5f;
	GLfloat blue = (sin(timeValue - 1) / 2.0f) + 0.5f;
	GLint vertexColorLocation = glGetUniformLocation(program, "cor");
	glUniform4f(vertexColorLocation, red, green, blue, 1.0f);
}