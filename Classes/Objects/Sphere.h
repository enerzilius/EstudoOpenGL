#ifndef SPHERE_CLASS_H
#define SPHERE_CLASS_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../glAbstractions/VBO.h"
#include "../Objects/Material.h"

class Sphere {
public:
	float radius;
	int resolution;
	int verticesCount;
	glm::vec3 color;
	std::vector<float> vertices;
	Material material;

	Sphere(float radius, int resolution, glm::vec3 color, Material objMaterial);
	void generateVertices(std::vector<float>& vertices, float radius, int resolution);
	VBO getVBO();
};

#endif
