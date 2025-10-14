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
#include "Classes/Utils/GeneralUtilities.h"
#include "Classes/Objects/Sphere.h"
#include "Classes/Objects/Material.h"

using namespace std;

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

void resize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void renderScene(vector<glm::vec3> positions, Shader& program, glm::mat4 model, int vertexCount);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float CLIP_NEAR = 0.1f;
float CLIP_FAR = 200.0f;

const float radius = 10.0;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 70.0f;
float movementSpeed = 2.5f;

glm::vec3 cameraPosition = glm::vec3(-10.0, 0.0, 0.0);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

const float sensitivity = 0.5f;

glm::vec3 worldUp = glm::vec3(0.0, 1.0, 0.0);
Camera camera(cameraPosition, worldUp, yaw, pitch, fov, sensitivity, movementSpeed);

vector<glm::vec3> positions = generateRandomPositions(4);

bool paused = false;
float paused_time = 0;

const char* wallPath = "Textures/wall.jpg";
const char* awesomePath = "Textures/awesomeface.png";

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

	Material awesome = Material(glm::vec3(1.0), glm::vec3(1.0), glm::vec3(1.0), 20.0, awesomePath, "", "");
	Material wall = Material(glm::vec3(1.0), glm::vec3(1.0), glm::vec3(1.0), 50.0, wallPath, "", "");
	Material jade = Material(glm::vec3(0.135, 0.2225, 0.1575), glm::vec3(0.54, 0.89, 0.63), glm::vec3(0.316228, 0.316228, 0.316228), 0.1, "", "", "");
	Material obsidian = Material(glm::vec3(0.05375, 0.05, 0.06625), glm::vec3(0.18275, 0.17, 0.22525), glm::vec3(0.332741, 0.328634, 0.346435), 0.3, "", "", "");
	Material silver = Material(glm::vec3(0.19225, 0.19225, 0.19225), glm::vec3(0.50754, 0.50754, 0.50754), glm::vec3(0.508273, 0.508273, 0.508273), 0.4, "", "", "");
	Material caixa = Material(glm::vec3(0.19225, 0.19225, 0.19225), glm::vec3(0.50754, 0.50754, 0.50754), glm::vec3(0.508273, 0.508273, 0.508273), 7, "Textures/container2.png", "Textures/container2_specular.png", "");
	Material caixaGlow = Material(glm::vec3(0.19225, 0.19225, 0.19225), glm::vec3(0.50754, 0.50754, 0.50754), glm::vec3(0.508273, 0.508273, 0.508273), 7, "Textures/container2.png", "Textures/container2_specular.png", "Textures/matrix.jpg");
	vector<Material> materialList = {
		awesome,
		wall,
		Material(glm::vec3(0.0215, 0.1745, 0.0215),	glm::vec3(0.07568, 0.61424, 0.07568),glm::vec3(0.633, 0.727811, 0.633), 6, "", "", ""),
		jade,
		obsidian,
		silver,
		caixa,
		caixaGlow
	};

	float radius = 1.33f;
	int sphereResolution = 30;
	glm::vec3 objColor = glm::vec3(1.0, 0.0, 0.0);
	Sphere sphere(radius, sphereResolution, materialList[6]);

	Shader shaderProgram("Shaderfiles/3d.vert", "Shaderfiles/flashlight.frag");

	//Shader bloomProgram("Shaderfiles/3d.vert", "Shaderfiles/blur.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1 = sphere.getVBO();
	//EBO EBO1(sqrIndices, sizeof(sqrIndices));
 
	int layoutVertex = 0, layoutUV = 1, layoutNormal = 2;
	int stepVertex = 3, stepUV = 2, stepNormal = 3;
	//int stride = stepVertex + stepUV + stepNormal;
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

	// BLOOM TEXTURE
	//Texture bloom(1);
	//Texture bloom(1);
	//bloom.Bind();
	//bloom.LinkTexBuffer(SCR_WIDTH, SCR_HEIGHT);
	//bloom.SetTexParameters();
	//int mipmapLevels = 0; // 4
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bloom.ID, mipmapLevels);

	// FAZER COM O DOWNSCALE DEPOIS
	//Texture downscaledBloom(1);
	//downscaledBloom.Bind();
	//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, SCR_WIDTH/32, SCR_HEIGHT/32, 0);

	

	glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);
	float cutoffAngle = glm::cos(glm::radians(9.0f));
	shaderProgram.Activate();
	shaderProgram.setVec3Float("lightColor", lightColor);

	lightShaderProgram.Activate();
	lightShaderProgram.setVec3Float("objectColor", lightColor);

	//loop de renderização
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.05f, 0.05f, 0.13f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		shaderProgram.setBool("usesDiffuseMap", sphere.material.usesDiffuseMap);
		shaderProgram.setBool("usesSpecularMap", sphere.material.usesSpecularMap);
		shaderProgram.setBool("usesGlowMap", sphere.material.usesGlowMap);
		if (sphere.material.usesDiffuseMap) {
			shaderProgram.setInt("diffuseMap", 0);
			sphere.material.diffuseMap.ActiveTexture(GL_TEXTURE0);
			sphere.material.diffuseMap.Bind();
		}if (sphere.material.usesSpecularMap) {
			shaderProgram.setInt("specularMap", 1);
			sphere.material.specularMap.ActiveTexture(GL_TEXTURE1);
			sphere.material.specularMap.Bind();
		}if (sphere.material.usesGlowMap) {
			shaderProgram.setInt("glowMap", 2);
			sphere.material.glowMap.ActiveTexture(GL_TEXTURE2);
			sphere.material.glowMap.Bind();
		}

		shaderProgram.setVec3Float("diffuseColor", sphere.material.diffuse);
		shaderProgram.setVec3Float("ambientColor", sphere.material.ambient);
		shaderProgram.setVec3Float("specularColor", sphere.material.specular);
		shaderProgram.setFloat("shininess", sphere.material.shininess);

		//shaderProgram.setVec3Float("lightpos", lightPos);
		shaderProgram.setVec3Float("lightpos", camera.position);
		shaderProgram.setVec3Float("lightDirection", camera.front);
		shaderProgram.setFloat("cutoffAngle", cutoffAngle);
		shaderProgram.setVec3Float("camPos", camera.position);
		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("proj", proj);

		VAO1.Bind();

		renderScene(positions, shaderProgram, model, sphere.verticesCount);

		VAO1.Unbind();

		glm::mat4 lightModel = glm::mat4(1.0);
		glm::translate(lightModel, lightPos);

		lightShaderProgram.Activate();
		vaoLight.Bind();

		lightShaderProgram.setMat4("model", lightModel);
		lightShaderProgram.setMat4("view", view);
		lightShaderProgram.setMat4("proj", proj);

		//glDrawArrays(GL_TRIANGLES, 0, sphere.verticesCount);

		vaoLight.Unbind();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	VAO1.Delete();
	VBO1.Delete();
	//EBO1.Delete();
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
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && (float)glfwGetTime() - paused_time > 0.5) {
		paused = !paused;
		cout << (paused ? "true" : "false") << "\n";
		if (paused) paused_time = (float)glfwGetTime();
	}
}

void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void renderScene(vector<glm::vec3> positions, Shader& program, glm::mat4 model, int vertexCount) {
	float angle = (float)glfwGetTime() - paused_time;
	if(paused) angle = paused_time;
	int i = 0;
	for (glm::vec3 pos : positions) {
		i++;
		angle /= i;
		float rotationX = pos.x * cos(angle);
		float amplitude = 3.0;
		float rotationY = sin(angle) * amplitude;
		float rotationZ = pos.x * sin(angle)-3.5;

		glm::vec3 position = glm::vec3(rotationX, rotationY, rotationZ);
		model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		//model = glm::rotate(model, glm::radians(((float)glfwGetTime()) * 50), glm::vec3(0.0f, 0.0f, 1.0f));

		program.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	if (camera.firstTouch)
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
