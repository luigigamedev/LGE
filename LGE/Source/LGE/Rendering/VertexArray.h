#pragma once

namespace LGE
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexBufferLayout;

	class VertexArray
	{
	private:
		unsigned int m_Id;
	public:
		// Without IBO - use glDrawArrays
		VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout);

		// With IBO - use glDrawElements
		VertexArray(const VertexBuffer& vb, const IndexBuffer& ib, const VertexBufferLayout& layout);

		~VertexArray();

		void Bind() const;
		void Unbind() const;
	};
}