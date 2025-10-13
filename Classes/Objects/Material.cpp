#include "Material.h"

#include <glm/glm.hpp>
#include <stb/stb_image.h>
#include "../glAbstractions/Texture.h"
#include "../Utils/GeneralUtilities.h"

using namespace std;

Material::Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininessValue, const char* texturePath, const char* specularPath, const char* glowPath) {
	ambient = ambientColor;
	diffuse = diffuseColor;
	specular = specularColor;
	shininess = shininessValue;
	if (texturePath && std::strlen(texturePath) > 0) applyTexture(GL_TEXTURE0, diffuseMap, usesDiffuseMap, texturePath);
	if (texturePath && std::strlen(specularPath) > 0) applyTexture(GL_TEXTURE1, specularMap, usesSpecularMap, specularPath);
	if (texturePath && std::strlen(glowPath) > 0) applyTexture(GL_TEXTURE2, glowMap, usesGlowMap, glowPath);
}

void Material::applyTexture(GLuint textureBuffer, Texture& texture, bool& flag, const char* texturePath) {
	texture.ActiveTexture(textureBuffer);
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
	flag = true;
}