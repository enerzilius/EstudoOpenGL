#include "Sphere.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "../Utils/GeneralUtilities.h"
#include "../glAbstractions/VBO.h"

Sphere::Sphere(float sphereRadius, int renderResolution, glm::vec3 objColor) {
	radius = sphereRadius;
	resolution = renderResolution;
	color = objColor;

}

void generateVertices(std::vector<float>& vertices, float radius, int resolution) {
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
