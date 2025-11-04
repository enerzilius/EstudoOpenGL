#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "../Objects/Mesh.h"
#include "../glAbstractions/Texture.h"
#include "../glAbstractions/VAO.h"
#include "../glAbstractions/VBO.h"
#include "../glAbstractions/EBO.h"
#include "../Utils/GeneralUtilities.h"

using namespace std;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh() {
    this->vao = new VAO();
    this->vbo = new VBO(getFloatArrayFromVertexArray(vertices.data()), sizeof(vertices.data()));
    this->ebo = new EBO(indices.data(), sizeof(indices));
}