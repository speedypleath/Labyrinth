#pragma once

#include <VAO.h>
#include <VBO.h>
#include <EBO.h>
#include <Shader.h>
#include <OpenGL/gl3.h>

class Renderer
{
private:
    VAO *vao;
    VBO *vbo;
    VBO *colorVBO;
    VBO *instanceVBO;
    VBO *textureVBO;
    EBO *ebo;
    Shader *shader;
    int instanceCount;
    int verticesSize;
    int indicesSize;
public:
    Renderer(GLfloat *vertices, GLuint *indices, int verticesSize, int indicesSize);
    Renderer(VAO *vao, VBO *vbo, EBO *ebo, Shader *shader);
    ~Renderer();
    void instance(int **map, int x, int y, int distance, glm::mat4 transform);
    void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
    void drawInstanced(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, int codCol);
};

        