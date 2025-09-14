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


#include "Classes/glAbstractions/shaderClass.h"
#include "Classes/glAbstractions/VBO.h"
#include "Classes/glAbstractions/EBO.h"
#include "Classes/glAbstractions/VAO.h"
#include "Classes/glAbstractions/Texture.h"
#include "Classes/ViewElements/Camera.h"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void resize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void renderScene(vector<glm::vec3> positions, Shader& program, glm::mat4 model, int vertexCount);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

glm::vec3 sphericalToCartesian(float r, float theta, float phi);
void getSphereVertices(vector<float>& vertices, float radius, int resolution);
void insertVec3InVector(vector<float>& vector, glm::vec3);
void insertQuadVertexVectorTexture(vector<float>& vector, glm::vec3 vertices[4]);
glm::vec3 calculateFaceNormalVector(glm::vec3 triangle[3]);
glm::vec3 calculateVertexNormalVector(glm::vec3 triangle);
vector<glm::vec3> generateRandomPositions(int n);

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

vector<glm::vec3> positions = generateRandomPositions(1);


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

	vector<float> sphereVertices;
	float radius = 1.33f;
	int sphereResolution = 15;
	getSphereVertices(sphereVertices, radius, sphereResolution);
	int vertexCount = sphereVertices.size() / 5;
	cout << sphereVertices.size() << endl;
	cout << vertexCount << endl;

	Shader shaderProgram("Shaderfiles/3d.vert", "Shaderfiles/3d.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(sphereVertices.data(), (sphereVertices.size()*sizeof(float)));
	//EBO EBO1(sqrIndices, sizeof(sqrIndices));

	int layoutVertex = 0, layoutUV = 1, layoutNormal = 2;
	int stepVertex = 3, stepUV = 2, stepNormal = 3;
	int stride = stepVertex + stepUV;
	VAO1.LinkVBO(VBO1, layoutVertex, stepVertex, stride, 0); 
	VAO1.LinkVBO(VBO1, layoutUV, stepUV, stride, stepVertex);
	//VAO1.LinkVBO(VBO1, layoutNormal, stepNormal, stride, stepVertex+stepUV);

	VAO1.Unbind();
	//EBO1.Unbind();

	Shader lightShaderProgram("Shaderfiles/light.vert", "Shaderfiles/light.frag");
	VAO lightVAO;

	VAO vaoLight;
	vaoLight.Bind();

	vaoLight.LinkVBO(VBO1, layoutVertex, stepVertex, stride, 0);
	vaoLight.LinkVBO(VBO1, layoutUV, stepUV, stride, stepVertex);
	vaoLight.LinkVBO(VBO1, layoutNormal, stepNormal, stride, stepVertex + stepUV);

	vaoLight.Unbind();
	VBO1.Unbind();

	Texture texture;
	texture.ActiveTexture(GL_TEXTURE0);
	texture.Bind();
	texture.SetTexParameters();

	int width, height, ch;
	stbi_set_flip_vertically_on_load(true);
	const char* path = "Textures/wall.jpg";
	unsigned char* data = stbi_load(path, &width, &height, &ch, 0);

	if (data) texture.LinkTexJPG(width, height, data);
	else cout << "Erro ao carregar a imagem " << path << endl;
	stbi_image_free(data);

	Texture texture2;
	texture2.Bind();
	texture2.SetTexParameters();

	path = "Textures/awesomeface.png";
	data = stbi_load(path, &width, &height, &ch, 0);
	if (data) texture2.LinkTexPNG(width, height, data);
	else cout << "Erro ao carregar a imagem " << path << endl;
	stbi_image_free(data);

	glm::vec3 lightColor = glm::vec3(1.0, 0.0, 1.0);
	shaderProgram.Activate();
	shaderProgram.setInt("tex0", 0);
	shaderProgram.setInt("tex1", 1);
	shaderProgram.setFloat("mixParam", 0.5);
	shaderProgram.setVec3Float("lightColor", lightColor);

	lightShaderProgram.Activate();
	lightShaderProgram.setVec3Float("objectColor", lightColor);

	//loop de renderização
	while (!glfwWindowShouldClose(window))
	{
		//inputs
		processInput(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//aqui vai os processos de renderização
		glClearColor(0.05f, 0.05f, 0.13f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture.ActiveTexture(GL_TEXTURE0);
		texture.Bind();
		texture2.ActiveTexture(GL_TEXTURE1);
		texture2.Bind();

		glm::vec3 lightPos = glm::vec3(5.0, -1.0, -4.0);

		shaderProgram.Activate();
		VBO1.Bind();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(camera.position - cameraTarget);

		camera.right = glm::normalize(glm::cross(camera.up, cameraDirection));
		camera.up = glm::cross(cameraDirection, camera.right);

		view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);

		proj = glm::perspective(glm::radians(camera.fov), (float)(SCR_WIDTH / SCR_HEIGHT), CLIP_NEAR, CLIP_FAR);

		shaderProgram.setVec3Float("lightpos", lightPos);
		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("proj", proj);

		VAO1.Bind();

		renderScene(positions, shaderProgram, model, vertexCount);

		VAO1.Unbind();

		glm::mat4 lightModel = glm::mat4(1.0);
		glm::translate(lightModel, lightPos);

		lightShaderProgram.Activate();
		vaoLight.Bind();

		lightShaderProgram.setMat4("model", lightModel);
		lightShaderProgram.setMat4("view", view);
		lightShaderProgram.setMat4("proj", proj);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		vaoLight.Unbind();

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

void renderScene(vector<glm::vec3> positions, Shader& program, glm::mat4 model, int vertexCount) {
	for (glm::vec3 pos : positions) {
		glm::vec3 position = pos;
		model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		//model = glm::rotate(model, glm::radians(((float)glfwGetTime()) * 50), glm::vec3(1.0f, 0.0f, 0.0f));
		program.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
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
	float calculationResolution = (float)resolution;
	for (float i = 0.0f; i <= resolution; ++i) {
		float theta = (i / calculationResolution) * pi;
		float theta2 = (i + 1) / calculationResolution * pi;

		for (float j = 0.0f; j < resolution; ++j) {
			float phi = j / calculationResolution * 2 * pi;
			float phi2 = (j + 1) / calculationResolution * 2 * pi;

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

void insertQuadVertexVectorTexture(std::vector<float>& allVertices, glm::vec3 quadVertices[4]) {
	float uvCoordinates[6][2] = {
		{ 0.0f, 0.0f }, // v1
		{ 1.0f, 0.0f }, // v2
		{ 0.0f, 1.0f }, // v3

		{ 1.0f, 0.0f }, // v2
		{ 1.0f, 1.0f }, // v4
		{ 0.0f, 1.0f }  // v3
	};

	int indices[6] = { 0, 1, 2, 1, 3, 2 };

	glm::vec3 triangle[3] = { quadVertices[0], quadVertices[1], quadVertices[2] };
	glm::vec3 normalVector = calculateFaceNormalVector(triangle);

	for (int i = 0; i < 6; i++) {
		int idx = indices[i];
		insertVec3InVector(allVertices, quadVertices[idx]);
		for(int j = 0; j < 2; j++) allVertices.push_back(uvCoordinates[i][j]);
		//insertVec3InVector(allVertices, normalVector);
	}
}

glm::vec3 calculateFaceNormalVector(glm::vec3 face[3]) {
	return glm::cross((face[1] - face[0]), face[2] - face[0]);
}

glm::vec3 calculateVertexNormalVector(glm::vec3 vertex) {
	return glm::vec3(1.0);
}


vector<glm::vec3> generateRandomPositions(int n) {
	srand(static_cast <unsigned> (time(0)));
	vector<glm::vec3> positionVector;
	for (int i = 0; i < n; i++)
	{
		float randomOffset = rand() % 100 * 0.123;
		glm::vec3 randomPos = glm::vec3(rand() % 1, rand() % 3, rand() % 2) + randomOffset;
		positionVector.push_back(randomPos);
	}
	return positionVector;
}