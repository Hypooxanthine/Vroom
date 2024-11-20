#include "Vroom/Render/Abstraction/VertexArray.h"

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/VertexBuffer.h"
#include "Vroom/Render/Abstraction/VertexBufferLayout.h"

namespace vrm
{

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::VertexArray(VertexArray&& other)
	: m_RendererID(other.m_RendererID)
{
	other.m_RendererID = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other)
{
	if (this != &other)
	{
		m_RendererID = other.m_RendererID;
		other.m_RendererID = 0;
	}

	return *this;
}

VertexArray::~VertexArray()
{
	GLCall_nothrow(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	vb.bind();
	GLCall(glBindVertexArray(m_RendererID));
	const auto& elements = layout.getElements();
	GLintptr offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(
			i, 
			element.count, 
			element.type, 
			element.normalized, 
			layout.getStride(), 
			(GLvoid*)offset
			));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}

} // namespace vrm