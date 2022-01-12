#include <Renderer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Renderer::Renderer(GLfloat *vertices, GLuint *indices, int verticesSize, int indicesSize)
{
    this->shader = new Shader("resource/shader.vert", "resource/shader.frag");
    this->vao = new VAO();
    this->vbo = new VBO(vertices, verticesSize);
    this->ebo = new EBO(indices, indicesSize);
    vao->addBufferVec4(*vbo, false);
}

Renderer::Renderer(VAO *vao, VBO *vbo, EBO *ebo, Shader *shader)
{
    this->vao = vao;
    this->vbo = vbo;
    this->ebo = ebo;
    this->shader = shader;
}

Renderer::~Renderer(){
    delete this->vao;
    delete this->vbo;
    delete this->ebo;
}

void Renderer::instance(int **map, int x, int y, glm::mat4 transform){

    this->instanceCount = 0;
    for(int i = 0; i < x; i++)
        for(int j = 0; j < y; j++)
            if(map[i][j])
                this->instanceCount++;
    
    glm::vec4 Colors[instanceCount];
	for (int n = 0; n < instanceCount; n++)
	{
		float a = float(n) / 4.0f;
		float b = float(n) / 5.0f;
		float c = float(n) / 6.0f;
		Colors[n][0] = 0.35f + 0.30f * (sinf(a + 2.0f) + 1.0f);
		Colors[n][1] = 0.25f + 0.25f * (sinf(b + 3.0f) + 1.0f);
		Colors[n][2] = 0.25f + 0.35f * (sinf(c + 4.0f) + 1.0f);
		Colors[n][3] = 1.0f;
	}

	glm::mat4 MatModel[this->instanceCount];

    int k = 0;
    for(int i = 0; i < x; i++)
        for(int j = 0; j < y; j++)
            if(map[i][j] == 1)
                MatModel[k++] = glm::translate(glm::mat4(1.0f), glm::vec3(i * 200.0f, 0.0f, j * 200.0f)) * transform;

    instanceVBO = new VBO(MatModel, sizeof(MatModel));
    colorVBO = new VBO(Colors, sizeof(Colors));
    vao->addBufferVec4(*colorVBO, true);
    vao->addBufferMat4(*instanceVBO);
}

void Renderer::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix){
    shader->bind();
    shader->setMat4("projection", projectionMatrix);
    shader->setMat4("view", viewMatrix);
    shader->setMat4("model", glm::mat4(1.0f));
    vao->bind();
    glDrawElements(GL_TRIANGLES, ebo->getCount(), GL_UNSIGNED_INT, 0);
    vao->unbind();
}

void Renderer::drawInstanced(glm::mat4 viewMatrix, glm::mat4 projectionMatrix){
    vao->bind();
    vbo->bind();
    colorVBO->bind();
    instanceVBO->bind();
    ebo->bind();
    int codCol = 0;
    shader->bind();
    shader->setInt("codCol", codCol);
    shader->setMat4("viewMatrix", viewMatrix);
    shader->setMat4("projectionMatrix", projectionMatrix);
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, instanceCount);
}