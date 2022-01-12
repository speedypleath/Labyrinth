#pragma once

class EBO {
private:
    const unsigned int *indices;
    unsigned int count;
    unsigned int indicesBufferId;
public:
    EBO(const unsigned int* indices, unsigned int count);
    ~EBO();
    int getCount() const { return count; }
    void bind() const;
    void unbind() const;
};