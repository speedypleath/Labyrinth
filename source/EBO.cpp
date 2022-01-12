#include <EBO.h>
#include <errorHandle.h>
#include <OpenGL/gl.h>

EBO::EBO(const GLuint *indices, GLuint count) : indices(indices), count(count)
{
    GLCall(glGenBuffers(1, &indicesBufferId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW));
}

    
EBO::~EBO(){
    GLCall(glDeleteBuffers(1, &indicesBufferId));
}
void EBO::bind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, indicesBufferId));
}
void EBO::unbind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
