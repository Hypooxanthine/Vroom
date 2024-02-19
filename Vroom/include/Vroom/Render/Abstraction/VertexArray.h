#pragma once

class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	virtual ~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
private:
	unsigned int m_RendererID;
};