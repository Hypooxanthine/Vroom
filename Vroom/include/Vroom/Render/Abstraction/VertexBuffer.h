#pragma once

class VertexBuffer
{
public:

	/**
	 * @brief Constructs a VertexBuffer object.
	 * @param data Raw pointer to vertices data.
	 * @param size Size of data (sizeof(Vertex) * vertices count).
	 */
	VertexBuffer(const void* data, unsigned int size);

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	VertexBuffer(VertexBuffer&&);
	VertexBuffer& operator=(VertexBuffer&&);

	/**
	 * @brief Releases GPU memory.
	 */
	virtual ~VertexBuffer();

	/**
	 * @brief Binds this vertex buffer.
	 */
	void bind() const;

	/**
	 * @brief Unbinds this vertex buffer.
	 */
	void unbind() const;

private:
	unsigned int m_RendererID;
};