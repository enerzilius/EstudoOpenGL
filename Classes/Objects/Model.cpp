#include "Model.h"
#include "../glAbstractions/shaderClass.h"

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <vector>
#include <string>

Model::Model(char* path) {
	loadModel(path);
}

void Model::Draw(Shader& shader) {
	for (Mesh mesh : meshes) mesh.Draw(shader);
}

void Model::loadModel(std::string path) {
	//Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
}