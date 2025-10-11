#include "Material.h"

#include <glm/glm.hpp>
#include <stb/stb_image.h>
#include "../glAbstractions/Texture.h"
#include "../Utils/GeneralUtilities.h"

using namespace std;

Material::Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininessValue, const char* texturePath) {
	ambient = ambientColor;
	diffuse = diffuseColor;
	specular = specularColor;
	shininess = shininessValue;
	if (texturePath && std::strlen(texturePath) > 0) applyTexture(texturePath);
}

void Material::applyTexture(const char* texturePath) {
	texture.ActiveTexture(GL_TEXTURE0);
	texture.Bind();
	texture.SetTexParameters();
	int width, height, ch;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(texturePath, &width, &height, &ch, 0);
	if (data) {
		if(isPNG(texturePath)) texture.LinkTexPNG(width, height, data);
		else texture.LinkTexJPG(width, height, data);
	}
	else std::cout << "Erro ao carregar a imagem " << texturePath << std::endl;
	stbi_image_free(data);
}