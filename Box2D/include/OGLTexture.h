#pragma once
#include <string>
#include "Common.h"

class OGLTexture
{
private:
    GLuint Id;

public:
    OGLTexture(const std::string &Path);
    void SetWrappingClampToEdge();
    ~OGLTexture();
    void Bind(GLenum TextureUnit);
};