#include "VertexArray.h"

#include <glad/glad.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

namespace LGE
{
	static void ApplyLayout(const VertexBufferLayout& layout)
	{
		const auto& elements = layout.GetElements();
		size_t offset = 0;
		for (int i = 0; i < elements.size(); i++)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, elements[i].Count, GL_FLOAT, GL_FALSE, layout.GetStride(), (void*)offset);
			offset += elements[i].Count * sizeof(float);
		}
	}

	VertexArray::VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		glGenVertexArrays(1, &m_Id);
		Bind();    // VAO must be bound first — records all state set after this point
		vb.Bind(); // VBO bind recorded into VAO
		ApplyLayout(layout); // attribute layout recorded into VAO permanently
	}

	VertexArray::VertexArray(const VertexBuffer& vb, const IndexBuffer& ib, const VertexBufferLayout& layout)
	{
		glGenVertexArrays(1, &m_Id);
		Bind();    // VAO must be bound first — records all state set after this point
		vb.Bind(); // VBO bind recorded into VAO
		ib.Bind(); // IBO bind recorded into VAO
		ApplyLayout(layout); // attribute layout recorded into VAO permanently
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_Id);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_Id);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}
