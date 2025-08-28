#include <iostream>
#include <cstdlib>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


#include "shaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "Texture.h"
#include "Camera.h"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void resize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void renderCubes(vector<glm::vec3> cubePositions, Shader& program, glm::mat4 model);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

glm::vec3 sphericalToCartesian(float r, float theta, float phi);
void getSphereVertices(vector<float>& vertices, float radius, int resolution);
void insertVec3InVector(vector<float>& vector, glm::vec3);
void insertQuadVertexVectorTexture(vector<float>& vector, glm::vec3 vertices[4]);

float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


vector<glm::vec3> cubePositions = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
//int vertexCount = 36;

float CLIP_NEAR = 0.1f;
float CLIP_FAR = 100.0f;

const float radius = 10.0;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 70.0f;
float movementSpeed = 2.5f;

glm::vec3 cameraPosition = glm::vec3(sin(glfwGetTime()) * radius, 0.0, cos(glfwGetTime()) * radius);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

const float sensitivity = 0.5f;

glm::vec3 worldUp = glm::vec3(0.0, 1.0, 0.0);
Camera camera(cameraPosition, worldUp, yaw, pitch, fov, sensitivity, movementSpeed);

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	vector<float> verticesVector;
	getSphereVertices(verticesVector, 1, 5);

	float* sphereVertices = &verticesVector[0];

	int vertexCount = sizeof(sphereVertices) / (sizeof(GLuint) * 5);

	cout << vertexCount << endl;

	Shader shaderProgram("Shaderfiles/3d.vert", "Shaderfiles/3d.frag");

	VAO VAO1; 
	VAO1.Bind();

	VBO VBO1(cubeVertices, sizeof(cubeVertices));
	//EBO EBO1(sqrIndices, sizeof(sqrIndices));

	VAO1.LinkVBO(VBO1, 0, 3, 5, 0);
	VAO1.LinkVBO(VBO1, 1, 3, 5, 3);

	VAO1.Unbind();
	VBO1.Unbind();
	//EBO1.Unbind();

	Texture texture;
	texture.ActiveTexture(GL_TEXTURE0);
	texture.Bind();
	texture.SetTexParameters();

	int width, height, ch;
	stbi_set_flip_vertically_on_load(true);
	const char* path = "Textures/wall.jpg";
	unsigned char* data = stbi_load(path, &width, &height, &ch, 0);

	if (data) texture.LinkTexJPG(width, height, data);
	else cout << "Erro ao carregar a imagem "<<path<<endl;
	stbi_image_free(data);

	Texture texture2;
	texture2.Bind();
	texture2.SetTexParameters();

	path = "Textures/awesomeface.png";
	data = stbi_load(path, &width, &height, &ch, 0);
	if (data) texture2.LinkTexPNG(width, height, data);
	else cout << "Erro ao carregar a imagem " << path << endl;
	stbi_image_free(data);

	shaderProgram.Activate();
	shaderProgram.setInt("tex0", 0);
	shaderProgram.setInt("tex1", 1);
	shaderProgram.setFloat("mixParam", 0.5);

	glm::vec3 vec = { 0.0, 1.0, 2.0 };
	vector<float> vectorr;
	insertVec3InVector(vectorr, vec);
	cout << vectorr[1] << endl;
	
	//loop de renderização
	while (!glfwWindowShouldClose(window))
	{
		//inputs
		processInput(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//aqui vai os processos de renderização
		glClearColor(0.3f, 0.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture.ActiveTexture(GL_TEXTURE0);
		texture.Bind();
		texture2.ActiveTexture(GL_TEXTURE1);
		texture2.Bind();

		shaderProgram.Activate();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(camera.position - cameraTarget);

		
		camera.right = glm::normalize(glm::cross(camera.up, cameraDirection));
		camera.up = glm::cross(cameraDirection, camera.right);

		view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);

		proj = glm::perspective(glm::radians(camera.fov), (float)(SCR_WIDTH/SCR_HEIGHT), CLIP_NEAR, CLIP_FAR);

		//shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("proj", proj);

		VAO1.Bind();

		renderCubes(cubePositions, shaderProgram, model);

		//checar por eventos e trocar os buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	VAO1.Delete();
	VBO1.Delete();
	//EBO1.Delete();
	texture.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) deltaTime *= 4.0;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboardInput(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboardInput(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboardInput(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboardInput(RIGHT, deltaTime);
}

void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void renderCubes(vector<glm::vec3> cubePositions, Shader& program, glm::mat4 model) {
	srand(static_cast <unsigned> (time(0)));
	int i = 0;
	for (glm::vec3 cubePosition : cubePositions) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePosition);
		if (i % 2 == 0) model = glm::rotate(model, glm::radians(((float)glfwGetTime()) * 50), glm::vec3(1.0f, 0.3f, 0.5f));
		program.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		i++;
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	if (camera.firstTouch) // initially set to true
	{
		lastX = xPos;
		lastY = yPos;
		camera.firstTouch = false;

		return;
	}

	float deltaX = xPos - lastX;
	float deltaY = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	deltaX *= camera.sensitivity;
	deltaY *= camera.sensitivity;

	camera.ProcessMouseMovement(deltaX, deltaY, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

glm::vec3 sphericalToCartesian(float r, float theta, float phi) {
	glm::vec3 pos = glm::vec3(0.0);
	pos.x = r * sin(theta) * cos(phi);
	pos.y = r * cos(theta);
	pos.z = r * sin(theta) * sin(phi);
	return pos;
};

void getSphereVertices(vector<float>& vertices, float radius, int resolution) {
	constexpr float pi = glm::pi<float>();
	for (float i = 0.0; i < resolution; i++) {
		float theta = (i / resolution) * pi;
		float theta2 = (i+1 / resolution) * pi;
		
		for (float j = 0.0; j < resolution; j++) {
			float phi = j / resolution * 2 * pi;
			float phi2 = (j+1) / resolution * 2 * pi;

			glm::vec3 v1 = sphericalToCartesian(radius, theta, phi);
			glm::vec3 v2 = sphericalToCartesian(radius, theta, phi2);
			glm::vec3 v3 = sphericalToCartesian(radius, theta2, phi);
			glm::vec3 v4 = sphericalToCartesian(radius, theta2, phi2);

			glm::vec3 quadVertices[] = { v1, v2, v3, v4 };

			insertQuadVertexVectorTexture(vertices, quadVertices);
		}
	}
}

void insertVec3InVector(vector<float>& vector, glm::vec3 vertex) {
	for (int i = 0; i < 3; i++) {
		vector.push_back(vertex[i]);
	}
}

void insertQuadVertexVectorTexture(vector<float>& allVertices, glm::vec3 quadVertices[4]) {
	float textureVertexMatrix[6][2] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f }
	};
	int triangle = 0;
	for (int i = 0; i < 6; i++) {
		if (i >= 3) triangle = 2;
		insertVec3InVector(allVertices, quadVertices[i-triangle]);
		for (int j = 0; j < 2; j++) allVertices.push_back(textureVertexMatrix[i][j]);
	}
}