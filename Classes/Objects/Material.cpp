#include "Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include "../glAbstractions/Texture.h"

Material::Material(glm::vec3 ambientColor = glm::vec3(0.5), glm::vec3 diffuseColor = glm::vec3(0.5), glm::vec3 specularColor = glm::vec3(0.5), float shininessValue = 500, const char* texturePath = "") {
	ambient = ambientColor;
	diffuse = diffuseColor;
	specular = specularColor;
	shininess = shininessValue;
	if (texturePath != "") applyTexture(texturePath);
}

void Material::applyTexture(const char* texturePath) {
	texture.ActiveTexture(GL_TEXTURE0);
	texture.Bind();
	texture.SetTexParameters();

	int width, height, ch;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(texturePath, &width, &height, &ch, 0);

	if (data) texture.LinkTexJPG(width, height, data);
	else std::cout << "Erro ao carregar a imagem " << texturePath << std::endl;
	stbi_image_free(data);
}