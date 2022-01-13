#pragma once

class Texture
{
private:
    unsigned int texture;
    unsigned int loadTexture(const char *filePath);
public:
    Texture(const char *filePath);
    ~Texture();
    void bind() const;
    void unbind() const;
};