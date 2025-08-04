#include <iostream>
#include <cstdlib>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "Texture.h"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void resize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void colorLooping(GLuint program);
void vertexTranslation(GLuint program);
void transformation(GLuint program);
void transformation2(GLuint program);
void fractal(GLuint program, int depth);

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
	// positions         // colors         // tex coords
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.5f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,-0.5f, 0.0f,   // bottom left
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.5f, 1.5f,  // top 
};

GLuint triangleIndices[] = {
	0, 1, 2,
};

float texCoords[] = {
	0.0f, 0.0f,  // lower-left corner  
	1.0f, 0.0f,  // lower-right corner
	0.5f, 1.0f   // top-center corner
};

float zoom = -1.0;

float sqr[] = {
	// positions          // colors           // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f+zoom, 1.0f+zoom,   // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f+zoom, 0.0f-zoom,   // bottom right
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f-zoom, 0.0f-zoom,   // bottom left
   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f-zoom, 1.0f+zoom    // top left 
};

GLuint sqrIndices[] = {
	0, 2, 1,
	0, 3, 2
};

int vertexCount = sizeof(sqrIndices) / sizeof(GLuint);

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Omor", NULL, NULL);

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

	Shader shaderProgram("3d.vert", "3d.frag");

	VAO VAO1; 
	VAO1.Bind();

	VBO VBO1(sqr, sizeof(sqr));
	EBO EBO1(sqrIndices, sizeof(sqrIndices));

	VAO1.LinkVBO(VBO1, 0, 3, 8, 0);
	VAO1.LinkVBO(VBO1, 1, 3, 8, 3);
	VAO1.LinkVBO(VBO1, 2, 2, 8, 6);

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Texture texture;
	texture.ActiveTexture(GL_TEXTURE0);
	texture.Bind();
	texture.SetTexParameters();

	int width, height, ch;
	stbi_set_flip_vertically_on_load(true);
	const char* path = "wall.jpg";
	unsigned char* data = stbi_load(path, &width, &height, &ch, 0);

	if (data) texture.LinkTexJPG(width, height, data);
	else cout << "Erro ao carregar a imagem "<<path<<endl;
	stbi_image_free(data);

	Texture texture2;
	texture2.Bind();
	texture2.SetTexParameters();

	path = "awesomeface.png";
	data = stbi_load(path, &width, &height, &ch, 0);
	if (data) texture2.LinkTexPNG(width, height, data);
	else cout << "Erro ao carregar a imagem " << path << endl;
	stbi_image_free(data);

	shaderProgram.Activate();
	shaderProgram.setInt("tex0", 0);
	shaderProgram.setInt("tex1", 1);
	shaderProgram.setFloat("mixParam", 0.5);

	//loop de renderização
	while (!glfwWindowShouldClose(window))
	{
		//inputs
		processInput(window);

		//aqui vai os processos de renderização
		glClearColor(0.3f, 0.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(50.0f), (float)(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);

		int modelLocation = glGetUniformLocation(shaderProgram.ID, "model");
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE glm::value_ptr)
		
		texture.ActiveTexture(GL_TEXTURE0);
		texture.Bind();
		texture2.ActiveTexture(GL_TEXTURE1);
		texture2.Bind();

		shaderProgram.Activate();
		VAO1.Bind();

		//fractal(shaderProgram.ID, 3);

		transformation(shaderProgram.ID);
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
		
		transformation2(shaderProgram.ID);
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);


		//checar por eventos e trocar os buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	texture.Delete();
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

void colorLooping(GLuint program) {
	GLfloat timeValue = 0.3 * glfwGetTime();
	GLfloat red = (sin(timeValue + 1) / 2.0f) + 0.5f;
	GLfloat green = (sin(timeValue) / 2.0f) + 0.5f;
	GLfloat blue = (sin(timeValue - 1) / 2.0f) + 0.5f;
	GLint vertexColorLocation = glGetUniformLocation(program, "cor");
	glUniform4f(vertexColorLocation, red, green, blue, 1.0f);
}

void vertexTranslation(GLuint program) {
	GLfloat timeValue = 0.3 * glfwGetTime();
	GLfloat offset = sin(timeValue)*0.5;
	GLint vertexOffset = glGetUniformLocation(program, "offset");
	glUniform3f(vertexOffset, offset, offset, 0);
}

void transformation(GLuint program) {
	GLfloat timeValue = glfwGetTime();
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm ::vec3(0, cos(timeValue)*0.5, 0.0f));
	trans = glm::rotate(trans, glm::radians(timeValue*100), glm::vec3(0.0, 0.0, 1.0));

	unsigned int transformLoc = glGetUniformLocation(program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void transformation2(GLuint program) {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(-0.9f, 0.9f, 0.0f));
	float scaleAmount = static_cast<float>(sin(glfwGetTime()))*0.5;
	trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
	unsigned int transformLoc = glGetUniformLocation(program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void fractal(GLuint program, int depth) {
	glm::mat4 trans = glm::mat4(1.0f);
	for (int i = 0; i < depth; i++) {
		srand(static_cast <unsigned> (time(0)));
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/1);
		float s = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/1);
		float theta = rand() % 360;
		float phi = rand() % 360;
		float h = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/1);
		float k = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/1);

		float x = r * cos(theta) + s * sin(phi) + h;
		float y = r * cos(theta) + s * sin(phi) + h;

		unsigned int transformLoc = glGetUniformLocation(program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);


	}
}