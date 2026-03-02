#pragma once

#include <vector>

namespace LGE
{
	// Describes a single vertex attribute (e.g. position, normal, texcoord)
	struct VertexBufferElement
	{
		int Count; // Number of floats: Float2 = 2, Float3 = 3, etc.
		// TODO: Add Type field to support non-float attributes
	};

	// Describes the full layout of a vertex — a list of attributes and the total stride.
	// VertexBufferLayout is a pure descriptor, it holds no GL state and makes no GL calls.
	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> m_Elements;
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

		const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		int GetStride() const { return m_Stride; }
	};
}
