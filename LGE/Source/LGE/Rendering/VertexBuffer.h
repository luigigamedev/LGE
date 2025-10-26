#pragma once

namespace LGE
{
	class VertexBuffer
	{
	private:
		unsigned int m_Id;

	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
	};
}
