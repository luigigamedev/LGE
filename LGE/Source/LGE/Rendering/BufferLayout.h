#pragma once

#include <vector>

namespace LGE
{
	// Describes a single vertex attribute (e.g. position, normal, texcoord)
	struct BufferElement
	{
		int Count; // Number of floats: Float2 = 2, Float3 = 3, etc.
		// TODO: Add Type field to support non-float attributes
	};

	// Describes the full layout of a vertex — a list of attributes and the total stride.
	// BufferLayout is a pure descriptor, it holds no GL state and makes no GL calls.
	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_Elements;
		int m_Stride = 0;
	public:
		void PushFloat2()
		{
			m_Elements.push_back({ 2 });
			m_Stride += 2 * sizeof(float);
		}

		void PushFloat3()
		{
			m_Elements.push_back({ 3 });
			m_Stride += 3 * sizeof(float);
		}

		const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		int GetStride() const { return m_Stride; }
	};
}
