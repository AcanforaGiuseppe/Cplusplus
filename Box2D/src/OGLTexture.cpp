#include "OGLTexture.h"
#include <glad/glad.h>
#include "stb_image.h"

static GLuint CreateTexture(const std::string &InImgPath)
{
    stbi_set_flip_vertically_on_load(true);
    int Width, Height, Channels;
    unsigned char *Data = stbi_load(InImgPath.c_str(), &Width, &Height, &Channels, 0);
    DIE_ON_NULL(Data, "Failed Loading Texture");

    GLenum Format = Channels == 3 ? GL_RGB : GL_RGBA;
    GLuint TextureId;
    glGenTextures(1, &TextureId);
    glBindTexture(GL_TEXTURE_2D, TextureId);

    // Load Data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Mimapping (Optional)
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(Data);
    stbi_set_flip_vertically_on_load(false);
    return TextureId;
}

OGLTexture::OGLTexture(const std::string &Path)
{
    Id = CreateTexture(Path);
}

OGLTexture::~OGLTexture()
{
    glDeleteTextures(1, &Id);
}

void OGLTexture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_2D, Id);
}

void OGLTexture::SetWrappingClampToEdge()
{
    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}