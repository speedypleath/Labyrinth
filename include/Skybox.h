#include <VAO.h>
#include <Shader.h>

class Skybox {
    VAO *vao;
    VBO *vbo;
    Shader *shader;
    unsigned int texture;
public:
    Skybox();
    ~Skybox();
    void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
};