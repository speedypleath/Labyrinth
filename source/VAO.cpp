#include <VAO.h>
#include <errorHandle.h>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>

VAO::VAO(){
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));
    atribCount = 0;
    size = 0;
}
VAO::~VAO(){
    GLCall(glDisableVertexAttribArray(1));
    GLCall(glDisableVertexAttribArray(0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GLCall(glBindVertexArray(0));
    GLCall(glDeleteVertexArrays(1, &vao));
}
void VAO::bind(){
    GLCall(glBindVertexArray(vao));
}
void VAO::unbind(){
    GLCall(glBindVertexArray(0));
}
void VAO::addBufferVec2(VBO& vbo){
    bind();
    vbo.bind();
    GLCall(glEnableVertexAttribArray(atribCount));
    GLCall(glVertexAttribPointer(atribCount, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0));
    atribCount++;
}
void VAO::addBufferVec4(VBO& vbo, bool withDivisor){
    vbo.bind();

    GLCall(glEnableVertexAttribArray(atribCount));
    GLCall(glVertexAttribPointer(atribCount, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0));
    if (withDivisor){
        GLCall(glVertexAttribDivisor(atribCount, 1));
    }
    atribCount++;
}
void VAO::addBufferMat4(VBO& vbo){
    vbo.bind();

	for (int i = 0; i < 4; i++) // Pentru fiecare coloana
	{
		glEnableVertexAttribArray(atribCount + i);
		glVertexAttribPointer(atribCount + i,              // Location
			4, GL_FLOAT, GL_FALSE,                // vec4
			sizeof(glm::mat4),                    // Stride
			(void*)(sizeof(glm::vec4) * i));      // Start offset
		glVertexAttribDivisor(atribCount + i, 1);
	}

    atribCount += 4;
}
