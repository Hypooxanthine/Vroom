#include "Vroom/Render/Abstraction/VertexBuffer.h"

#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm
{

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	: m_RendererID(0)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(VertexBuffer&& other)
	: m_RendererID(other.m_RendererID)
{
	other.m_RendererID = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
{
	if (this != &other)
	{
		m_RendererID = other.m_RendererID;
		other.m_RendererID = 0;
	}

	return *this;
}

VertexBuffer::~VertexBuffer()
{
	GLCall_nothrow(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

} // namespace vrm