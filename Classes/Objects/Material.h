#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../glAbstractions/Texture.h"

class Material {
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	Texture texture;

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, const char* texturePath);
	void applyTexture(const char* texturePath);
};

#endif
