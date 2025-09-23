#include "Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Material::Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininessValue) {
	ambient = ambientColor;
	diffuse = diffuseColor;
	specular = specularColor;
	shininess = shininessValue;
}