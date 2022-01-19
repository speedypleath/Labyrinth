#include <Skybox.h>
#include <vector>
#include <string>
#include <stb_image.h>
#include <OpenGL/gl3.h>
#include <errorHandle.h>
#include <iostream>

Skybox::Skybox() {
    float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };

    vao = new VAO();
    vbo = new VBO(skyboxVertices, sizeof(skyboxVertices));
    shader = new Shader("resource/skybox.vert", "resource/skybox.frag");
    vao->addBufferVec3(*vbo);

        std::vector<std::string> faces
    {
        "resource/posx.jpg",
        "resource/negx.jpg",
        "resource/posy.jpg",
        "resource/negy.jpg",
        "resource/posz.jpg",
        "resource/negz.jpg"
    };
    GLCall(glGenTextures(1, &texture));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, texture));

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLCall(glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                0, 
                GL_RGB, 
                width, 
                height, 
                0, 
                GL_RGB, 
                GL_UNSIGNED_BYTE, 
                data
            ));
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
}

Skybox::~Skybox() {
    delete vao;
    delete shader;
}

void Skybox::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    vao->bind();
    vbo->bind();
    shader->bind(); 
    GLCall(glDepthMask(GL_FALSE));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, texture));
    GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
    shader->setMat4("projection", projectionMatrix);
    glm::mat4 view = glm::mat4(glm::mat3(viewMatrix));
    shader->setMat4("view", view);

    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
    GLCall(glDepthMask(GL_TRUE));
}