#pragma once

#include <vector>
#include <glad/glad.h>

namespace LGE
{
	struct BufferElement
	{
		// unsigned int Type; Only GL_FLOAT for now
		int Count; // Float = 1, Float2 = 2, Float3 = 3, Float4 = 4
	};
	
	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_Elements;
		int m_Stride = 0;
	public:
		void PushFloat2()
		{
			m_Elements.push_back({2});
			m_Stride += 2 * sizeof(float);
		}

		void PushFloat3()
		{
			m_Elements.push_back({3});
			m_Stride += 3 * sizeof(float);
		}
		
		void Attrib() const
		{
			int index = 0;
			size_t offset = 0;
			for (const auto& element : m_Elements)
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, element.Count, GL_FLOAT, GL_FALSE, m_Stride, (void*)offset);
				
				index++;
				offset += element.Count * sizeof(float);
				// TODO: Replace sizeof(float) to support other types
			}
		}
	};
}

