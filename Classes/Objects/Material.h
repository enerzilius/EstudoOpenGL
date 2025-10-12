#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include <glm/glm.hpp>
#include "../glAbstractions/Texture.h"

class Material {
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	Texture diffuseMap;
	Texture specularMap;
	bool usesTexture = false;

	Material() = default;
	Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininessValue, const char* texturePath, const char* specularPath);
	void applyTexture(Texture& texture, const char* texturePath);
};

#endif
