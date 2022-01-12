#include <VBO.h>
#include <errorHandle.h>
#include <OpenGL/gl.h>

VBO::VBO(const void *vertices, unsigned int size) : vertices(vertices){
    GLCall(glGenBuffers(1, &verticesBufferId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
}
    
VBO::~VBO(){
    GLCall(glDeleteBuffers(1, &verticesBufferId));
}
void VBO::bind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId));
}
void VBO::unbind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
