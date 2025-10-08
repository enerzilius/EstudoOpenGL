#include "Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include "../glAbstractions/Texture.h"
#include "../Utils/GeneralUtilities.h"

using namespace std;

Material::Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininessValue, const char* texturePath) {
	cout << ambientColor.x << diffuseColor.x << specularColor.x << shininessValue << texturePath << std::endl;
	ambient = ambientColor;
	diffuse = diffuseColor;
	specular = specularColor;
	shininess = shininessValue;
	if (texturePath != "") applyTexture(texturePath);
}

void Material::applyTexture(const char* texturePath) {
	cout << "aaaa\n";
	isPNG(texturePath);
	texture = Texture(1);
	texture.ActiveTexture(GL_TEXTURE0);
	texture.Bind();
	texture.SetTexParameters();
	cout << "aaaaaaaa\n";
	int width, height, ch;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(texturePath, &width, &height, &ch, 0);
	cout << "aaaaaaaaaaaaaaaaaaaaaaa\n";
	if (data) {
		if(isPNG(texturePath)) texture.LinkTexPNG(width, height, data);
		else texture.LinkTexJPG(width, height, data);
	}
	else std::cout << "Erro ao carregar a imagem " << texturePath << std::endl;
	stbi_image_free(data);
}