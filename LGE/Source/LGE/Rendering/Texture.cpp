#include "Texture.h"

#include <iostream>

#include <glad/glad.h>
#include "stb/stb_image.h"

namespace LGE 
{
    Texture::Texture(const std::string& path, bool repeat)
        : m_Id(0), m_FilePath(path), m_Width(0), m_Height(0), m_Channels(0)
    {
        stbi_set_flip_vertically_on_load(true);

        glGenTextures(1, &m_Id);
        glBindTexture(GL_TEXTURE_2D, m_Id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Wrap Mode
        if (repeat)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        
        unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);

        if (data)
        {
            GLenum internalFormat = 0;
            GLenum dataFormat = 0;

            if (m_Channels == 4)
            {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else if (m_Channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
            
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        glBindTexture(GL_TEXTURE_2D, 0); // Unbind
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_Id);
    }

    void Texture::Bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_Id);
    }

    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
