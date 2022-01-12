#include "Shader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <errorHandle.h>
#include <OpenGL/gl.h>

std::string Shader::readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}


GLuint Shader::loadShaders(const char *vertex_path, const char *fragment_path) {
    GLCall(GLuint vertShader = glCreateShader(GL_VERTEX_SHADER));
    GLCall(GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER));

    // Read shaders

    std::string vertShaderStr = readFile(vertex_path);
    std::string fragShaderStr = readFile(fragment_path);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    // Compile vertex shader

    std::cout << "Compiling vertex shader." << std::endl;
    GLCall(glShaderSource(vertShader, 1, &vertShaderSrc, NULL));
    GLCall(glCompileShader(vertShader));

    // Check vertex shader

    GLCall(glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result));
    GLCall(glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength));
    std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
    GLCall(glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]));
    std::cout << &vertShaderError[0] << std::endl;

    // Compile fragment shader

    std::cout << "Compiling fragment shader." << std::endl;
    GLCall(glShaderSource(fragShader, 1, &fragShaderSrc, NULL));
    GLCall(glCompileShader(fragShader));

    // Check fragment shader

    GLCall(glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result));
    GLCall(glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength));
    std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);
    GLCall(glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]));
    std::cout << &fragShaderError[0] << std::endl;

    std::cout << "Linking program" << std::endl;
    GLCall(GLuint program = glCreateProgram());
    GLCall(glAttachShader(program, vertShader));
    GLCall(glAttachShader(program, fragShader));
    GLCall(glLinkProgram(program));

    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
    GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength));
    std::vector<char> programError( (logLength > 1) ? logLength : 1 );
    GLCall(glGetProgramInfoLog(program, logLength, NULL, &programError[0]));
    std::cout << &programError[0] << std::endl;

    GLCall(glDeleteShader(vertShader));
    GLCall(glDeleteShader(fragShader));
    this->program = program;
    
    return program;
}


Shader::Shader(const char *vertex_path, const char *fragment_path) : program(0) {
    program = loadShaders(vertex_path, fragment_path);
    GLCall(glUseProgram(program));
}

Shader::~Shader(){
    GLCall(glDeleteProgram(program));
}
void Shader::setInt(const char *name, int value) const {
    unsigned int location = glGetUniformLocation(program, name);
    GLCall(glUniform1i(location, value));
}
void Shader::setFloat(const char *name, float value) const {
    unsigned int location = glGetUniformLocation(program, name);
    GLCall(glUniform1f(location, value));
}
void Shader::setMat4(const char *name, glm::mat4 value) const {
    unsigned int location = glGetUniformLocation(program, name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]));
}

void Shader::bind() const {
    GLCall(glUseProgram(program));
}

void Shader::unbind() const {
    GLCall(glUseProgram(0));
}