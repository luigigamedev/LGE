#pragma once

#include <string>

namespace LGE 
{
    class Texture
    {
    private:
        unsigned int m_Id;
        std::string m_FilePath;
        int m_Width;
        int m_Height;
        int m_Channels;
    public:
        Texture(const std::string& path, bool repeat);
        ~Texture();

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
        
        void Bind(unsigned int slot = 0) const;
        void Unbind() const;
    };
}
