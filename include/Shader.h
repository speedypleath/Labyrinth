#pragma once
#include <string>
#include <glm/glm.hpp>
  
class Shader
{
private:
    unsigned int program;
    unsigned int loadShaders(const char *vertex_path, const char *fragment_path);
    std::string readFile(const char *filePath);
public:
    Shader(const char *vertex_path, const char *fragment_path);
    ~Shader();
    void setInt(const char *name, int value) const;
    void setFloat(const char *name, float value) const;
    void setMat4(const char *name, glm::mat4 value) const;
    void setVec2(const char *name, glm::vec2 value) const;
    void bind() const;
    void unbind() const;
};
