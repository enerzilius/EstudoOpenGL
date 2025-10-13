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
	Texture glowMap;
	bool usesDiffuseMap = false;
	bool usesSpecularMap = false;
	bool usesGlowMap = false;


	Material() = default;
	Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininessValue, const char* texturePath, const char* specularPath, const char* glowPath);
	void applyTexture(GLuint textureBuffer, Texture& texture, bool& flag, const char* texturePath);
};

#endif
