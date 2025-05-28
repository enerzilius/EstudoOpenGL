#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void resize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void render(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"out vec4 FragColor;\n"
	"void main(){\n"
	"	FragColor = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
	"}\0";


int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "EberzaoOpenGL", NULL, NULL);

	glfwSetFramebufferSizeCallback(window, resize);

	if (window == NULL) {
		std::cout << "Falha na cria��o da janela" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	glfwSetFramebufferSizeCallback(window, resize);

	//coordenadas dos v�rtices do tri�ngulo
	//z = 0 para fazer uma imagem 2d
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	//buffer para enviar os v�rtices para a GPU de uma vez
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//vertex shader, n�mero de strings source, a origem e outro que eu n�o sei
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//checar se a compila��o do shader deu certo
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	//sucess, definido com glGetShaderiv, � checado aqui
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//mesma coisa do vertex shader s� que com o fragmentShader
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragShader);

	if (!success)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Um shaderprogram � a vers�o final de v�rios shaders combinados
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//para utiliz�-lo, � preciso conectar ele com os shaders:
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	//deletar os shaders usados ap�s o link
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	//checar por erro no shaderProgram
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//tudo ap�s isso utiliza o programa para renderiza��o
	glUseProgram(shaderProgram);

	//loop de renderiza��o
	while (!glfwWindowShouldClose(window))
	{
		//inputs
		processInput(window);

		//aqui vai os processos de renderiza��o
		glClearColor(0.7f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//checar por eventos e trocar os buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

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