#pragma once

namespace vrm
{

class IndexBuffer
{
public:
	/**
	 * @brief Constructs an IndexBuffer object.
	 * @param data Raw pointer to indices data.
	 * @param count Total indices count (triangles count * 3).
	 */
	IndexBuffer(const unsigned int* data, unsigned int count);

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	IndexBuffer(IndexBuffer&&);
	IndexBuffer& operator=(IndexBuffer&&);

	/**
	 * @brief Releases GPU data.
	 */
	virtual ~IndexBuffer();

	/**
	 * @brief Binds buffer.
	 */
	void bind() const;

	/**
	 * @brief Unbinds buffer.
	 */
	void unbind() const;

	/**
	 * @brief Gets indices count.
	 * @return Indices count (triangles count * 3).
	 */
	inline unsigned int getCount() const { return m_Count; }

private:
	unsigned int m_RendererID;
	unsigned int m_Count;
};

} // namespace vrm