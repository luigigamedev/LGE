#pragma once

namespace LGE
{
	// Stores indices into a VBO, allowing vertex reuse.
	// Reduces GPU memory by avoiding duplicate vertices.
	class IndexBuffer
	{
	private:
		unsigned int m_Id;
		unsigned int m_Count; // number of indices, not bytes
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		unsigned int GetCount() const { return m_Count; }
	};
}