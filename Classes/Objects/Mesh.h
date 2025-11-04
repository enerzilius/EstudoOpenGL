#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../glAbstractions/Texture.h"
#include "../glAbstractions/shaderClass.h"
#include "../glAbstractions/EBO.h"
#include "../glAbstractions/VBO.h"
#include "../glAbstractions/VAO.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader& shader);

private:
    //  render data
    VBO* vbo;
    VAO* vao;
    EBO* ebo;

    void setupMesh();
};
