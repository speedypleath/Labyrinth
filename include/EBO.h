#pragma once

class EBO {
private:
    const unsigned int *indices;
    unsigned int count;
    unsigned int indicesBufferId;
public:
    EBO(const unsigned int* indices, unsigned int count);
    ~EBO();
    void bind() const;
    void unbind() const;
};