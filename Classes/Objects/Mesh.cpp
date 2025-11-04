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
    this->vbo = new VBO(&vertices[0].Normal.x, sizeof(vertices.data()));
    this->ebo = new EBO(indices.data(), sizeof(indices));

    vao->Bind();

    int layoutVertex = 0, layoutUV = 1, layoutNormal = 2;
    int stepVertex = 3, stepUV = 2, stepNormal = 3;
    int stride = stepVertex + stepUV + stepNormal;
    vao->LinkVBO(*vbo, layoutVertex, stepVertex, stride, 0);
    vao->LinkVBO(*vbo, layoutNormal, stepNormal, stride, stepVertex);
    vao->LinkVBO(*vbo, layoutUV, stepUV, stride, stepVertex+stepNormal);

    vao->Unbind();
}