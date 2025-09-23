#ifndef GENERALUTILITIES_CLASS_H
#define GENERALUTILITIES_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <vector>

std::vector<glm::vec3> generateRandomPositions(int n);
glm::vec3 sphericalToCartesian(float r, float theta, float phi);
void insertVec3InVector(std::vector<float>& vector, glm::vec3 vertex);
void insertQuadVertexVectorTexture(std::vector<float>& allVertices, glm::vec3 quadVertices[4]);
glm::vec3 calculateFaceNormalVector(glm::vec3 face[3]);

#endif
