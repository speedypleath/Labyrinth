#pragma once
#include <VBO.h>

class VAO {
private:
    unsigned int vao;
    unsigned int atribCount;
    unsigned int size;
public:
    VAO();
    ~VAO();
    void bind();
    void unbind();
    void addBufferVec4(VBO& vbo, bool withDivisor);
    void addBufferMat4(VBO& vbo);
};