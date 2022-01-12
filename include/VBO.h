#pragma once

class VBO {
private:
    const void *vertices;
    unsigned int vaoId;
    unsigned int verticesBufferId;
public:
    VBO(const void* vertices, unsigned int size);
    ~VBO();
    void bind() const;
    void unbind() const;
};