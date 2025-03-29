#pragma once

#include <vector>
#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm::gl
{

/**
 * @brief Stores a single vertex element in layout : type of element variable, variables count, normalized or not.
 */
struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static constexpr unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}

		VRM_ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
public:
	
	/**
	 * @brief Constructs an empty VertexBufferLayout object.
	 */
	VertexBufferLayout()
		: m_Stride(0)
	{}

	VertexBufferLayout(const VertexBufferLayout&) = default;
	VertexBufferLayout& operator=(const VertexBufferLayout&) = default;

	VertexBufferLayout(VertexBufferLayout&&) = default;
	VertexBufferLayout& operator=(VertexBufferLayout&&) = default;

	/**
	 * @brief Does nothing.
	 */
	virtual ~VertexBufferLayout();

	/**
	 * @brief Registers a float element to the layout.
	 * @param count Number of floats of the element.
	 */
	void pushFloat(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	/**
	 * @brief Registers an unsigned int element to the layout.
	 * @param count Number of unsigned ints of the element.
	 */
	void pushUInt(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	/**
	 * @brief Registers an unsigned char element to the layout.
	 * @param count Number of unsigned chars of the element.
	 */
	void pushUChar(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	/**
	 * @brief Gets the ordered list of elements of this layout.
	 * @return The list of elements.
	 */
	inline const std::vector<VertexBufferElement>& getElements() const { return m_Elements; }

	/**
	 * @brief Gets the stride of this layout.
	 * @return The stride of this layout.
	 */
	inline unsigned int getStride() const { return m_Stride; }

private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
};

} // namespace vrm::gl