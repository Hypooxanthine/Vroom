#pragma once

#include "Vroom/Render/Abstraction/VertexBuffer.h"
#include "Vroom/Render/Abstraction/Vertex.h"

namespace vrm::gl
{

class VertexBufferLayout;

class VertexArray
{
public:

	/**
	 * @brief Constructs a VertexArray object.
	 */
	VertexArray();

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	VertexArray(VertexArray&&);
	VertexArray& operator=(VertexArray&&);

	/**
	 * @brief Releases GPU memory.
	 */
	virtual ~VertexArray();

	/**
	 * @brief Adds a vertex buffer and its layout to this vertex array.
	 * @param vb Vertex buffer to add.
	 * @param layout Corresponding layout.
	 */
	void addBuffer(const VertexBuffer<Vertex>& vb, const VertexBufferLayout& layout);

	/**
	 * @brief Binds this vertex array.
	 */
	void bind() const;

	/**
	 * @brief Unbinds this vertex array.
	 */
	void unbind() const;

private:
	unsigned int m_RendererID;
};

} // namespace vrm::gl