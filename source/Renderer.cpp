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

void Renderer::instance(int **map, int x, int y, int distance, glm::mat4 transform){

    this->instanceCount = 0;
    for(int i = 0; i < x; i++)
        for(int j = 0; j < y; j++)
            if(map[i][j])
                this->instanceCount++;
    
    glm::vec4 colors[instanceCount];
    glm::vec2 texture[instanceCount];
	glm::mat4 matModel[instanceCount];

	for (int n = 0; n < instanceCount; n++)
	{
		float a = float(n) / 4.0f;
		float b = float(n) / 5.0f;
		float c = float(n) / 6.0f;
		colors[n][0] = 0.35f + 0.30f * (sinf(a + 2.0f) + 1.0f);
		colors[n][1] = 0.25f + 0.25f * (sinf(b + 3.0f) + 1.0f);
		colors[n][2] = 0.25f + 0.35f * (sinf(c + 4.0f) + 1.0f);
		colors[n][3] = 1.0f;
	}

    int k = 0;
    texture[0] = glm::vec3(1.0f, 1.0f, 0.0f);
    texture[1] = glm::vec3(0.0f, 1.0f, 0.0f);
    texture[2] = glm::vec3(0.0f, 0.0f, 0.0f);
    texture[3] = glm::vec3(1.0f, 0.0f, 0.0f);
    texture[4] = glm::vec3(0.0f, 0.0f, 0.0f);
    texture[5] = glm::vec3(1.0f, 0.0f, 0.0f);
    texture[6] = glm::vec3(1.0f, 1.0f, 0.0f);
    texture[7] = glm::vec3(0.0f, 1.0f, 0.0f);

    for(int i = 0; i < x; i++)
        for(int j = 0; j < y; j++)
            if(map[i][j] == 1){
                matModel[k++] = glm::translate(glm::mat4(1.0f), glm::vec3(distance * i, 0.0f, distance * j)) * transform;
        }

    instanceVBO = new VBO(matModel, sizeof(matModel));
    colorVBO = new VBO(colors, sizeof(colors));
    textureVBO = new VBO(texture, sizeof(texture));
    vao->addBufferVec4(*colorVBO, true);
    vao->addBufferVec2(*textureVBO);
    vao->addBufferMat4(*instanceVBO);
}

void Renderer::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix){
    vao->bind();
    vbo->bind();
    ebo->bind();
    int codCol = 0;
    shader->bind();
    shader->setInt("codCol", codCol);
    shader->setMat4("viewMatrix", viewMatrix);
    shader->setMat4("projectionMatrix", projectionMatrix);
    glDrawElements(GL_TRIANGLES, ebo->getCount(), GL_UNSIGNED_INT, 0);
    vao->unbind();
}

void Renderer::drawInstanced(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, int codCol){
    vao->bind();
    vbo->bind();
    colorVBO->bind();
    instanceVBO->bind();
    ebo->bind();
    shader->bind();
    int texture = 0;
    shader->setInt("tex_Unit", texture);
    shader->setInt("codCol", codCol);
    shader->setMat4("viewMatrix", viewMatrix);
    shader->setMat4("projectionMatrix", projectionMatrix);
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, instanceCount);
}