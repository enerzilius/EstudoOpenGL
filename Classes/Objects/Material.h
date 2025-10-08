#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include "../glAbstractions/Texture.h"
#include "../Utils/GeneralUtilities.h"

class Material {
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	Texture texture;

	Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininessValue, const char* texturePath);
	void applyTexture(const char* texturePath);
};

#endif
