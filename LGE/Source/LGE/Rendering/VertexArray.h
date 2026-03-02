#pragma once

namespace LGE
{
    class VertexBuffer;
    class BufferLayout;

    class VertexArray
    {
    private:
        unsigned int m_Id;
    public:
        VertexArray(const VertexBuffer& vb, const BufferLayout& layout);
        ~VertexArray();

        void Bind() const;
        void Unbind() const;
    };
}
