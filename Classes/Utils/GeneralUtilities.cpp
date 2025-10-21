#include "GeneralUtilities.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>

std::vector<glm::vec3> generateRandomPositions(int n) {
	srand(static_cast <unsigned> (0));
	std::vector<glm::vec3> positionVector;
	for (int i = 1; i <= n; i++)
	{
		glm::vec3 randomPos = glm::vec3(i * (2 * (i + 1)) + 4, 0, 0);
		positionVector.push_back(randomPos);
	}
	return positionVector;
}

glm::vec3 sphericalToCartesian(float r, float theta, float phi) {
	glm::vec3 pos = glm::vec3(0.0);
	pos.x = r * sin(theta) * cos(phi);
	pos.y = r * cos(theta);
	pos.z = r * sin(theta) * sin(phi);
	return pos;
};

void insertVec3InVector(std::vector<float>& vector, glm::vec3 vertex) {
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
		for (int j = 0; j < 2; j++) allVertices.push_back(uvCoordinates[i][j]);
		//insertVec3InVector(allVertices, normalVector);
	}
}

glm::vec3 calculateFaceNormalVector(glm::vec3 face[3]) {
	return glm::cross((face[1] - face[0]), face[2] - face[0]);
}

std::vector<std::string> split(std::string& string, std::string& delimiter) {
	std::vector<std::string> res;
	int i = 0;
	while ((i = string.find(delimiter)) != std::string::npos) {
		std::string word = string.substr(0, i);
		res.push_back(word);
		string.erase(0, i + delimiter.length());
	}
	res.push_back(string);
	return res;
}

bool isPNG(std::string path) {
	std::string delimiter = ".";
	return split(path, delimiter)[1] == "png";
}

void printVec3(glm::vec3 vector) {
	for (int i = 0; i < 3; i++) std::cout << vector[i] << " ";
	std::cout << "\n";
}

float* getFloatArrayFromVertexArray(Vertex* vertices) {
	std::vector<float> verticesData;
	int n = sizeof(vertices) / sizeof(Vertex);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 3; j++) verticesData.push_back(vertices[i].Normal[j]);
		for (int j = 0; j < 3; j++) verticesData.push_back(vertices[i].Position[j]);
		for (int j = 0; j < 2; j++) verticesData.push_back(vertices[i].TexCoords[j]);
	}
	return verticesData.data();
}