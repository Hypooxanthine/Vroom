#pragma once

#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm
{

template <GLenum TARGET>
class BufferGL
{
public:

    /**
     * @brief Construct a new BufferGL object.
     * 
     */
    inline constexpr BufferGL() = default;

    /**
     * @brief Copy constructor. Copies the buffer data to a new buffer.
     * 
     * @param other The other buffer
     */
    inline explicit BufferGL(const BufferGL& other)
    {
        *this = other;
    }

    /**
     * @brief Move constructor. Moves the buffer data to a new buffer.
     * 
     * @param other The other buffer
     */
    inline explicit BufferGL(BufferGL&& other) noexcept
    {
        *this = std::move(other);
    }

    /**
     * @brief Destroy the buffer object. Deletes the buffer if it was generated.
     * 
     */
    inline virtual ~BufferGL() noexcept
    {
        release();
    }

    /**
     * @brief Copy assignment operator. Copies the buffer data to a new buffer.
     * 
     * @param other The other buffer
     * @return BufferGL& This buffer
     */
    inline BufferGL& operator=(const BufferGL& other)
    {
        if (this == &other)
            return *this;

        release();

        if (other.isValid())
        {
            glBindBuffer(GL_COPY_READ_BUFFER, other.getRenderId());

            GLint size;
            glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
            GLint usage;
            glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_USAGE, &usage);

            generate(nullptr, (GLenum)size, (GLint)usage);
            glBindBuffer(GL_COPY_WRITE_BUFFER, getRenderId());

            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * 
     * @param other The other buffer
     * @return BufferGL& This buffer
     */
    inline BufferGL& operator=(BufferGL&& other) noexcept
    {
        if (this == &other)
            return *this;
            
        release();

        if (other.isValid())
        {
            m_RenderID = other.m_RenderID;
            other.m_RenderID = 0;
        }

        return *this;
    }

    /**
     * @brief Create a buffer on gpu.
     */
    inline GLuint generate()
    {
        release();
        glGenBuffers(1, &m_RenderID);

        return m_RenderID;
    }

    /**
     * @brief (Re)assign buffer data.
     * 
     * @param data The data
     * @param size Data size (byte count)
     * @param usage GL_STREAM_DRAW, GL_STATIC_DRAW, or GL_DYNAMIC_DRAW
     */
    inline void setData(const void* data, GLsizeiptr size, GLenum usage)
    {
        bind();
        glBufferData(TARGET, size, data, usage);
    }

    /**
     * @brief Set buffer sub data. Be careful with buffer size you set.
     * 
     * @param data The data
     * @param size Data size (byte count)
     * @param offset Where the data should be in the buffer (in bytes)
     */
    inline void setSubData(const void* data, GLsizeiptr size, GLintptr offset)
    {
        bind();
        glBufferSubData(TARGET, offset, size, data);
    }

    /**
     * @brief Create a buffer and set its data
     * 
     * @param data The data
     * @param size Data size (byte count)
     * @param usage GL_STREAM_DRAW, GL_STATIC_DRAW, or GL_DYNAMIC_DRAW
     * @return GLuint The render id of the created buffer
     */
    inline GLuint generate(const void* data, GLsizeiptr size, GLenum usage)
    {
        generate();
        setData(data, size, usage);
        return m_RenderId;
    }

    /**
     * @brief Deletes the buffer. Does nothing if the buffer was not generated.
     * 
     */
    inline void release()
    {
        glDeleteBuffers(1, &m_RenderID);
        m_RenderID = 0;
    }

    /**
     * @brief Binds a buffer of target TARGET.
     * 
     * @param renderId Render id of the buffer
     */
    inline static void Bind(GLuint renderId)
    {
        glBindBuffer(TARGET, renderId);
    }

    /**
     * @brief Unbinds the currently bound buffer of target TARGET.
     * 
     */
    inline static void Unbind()
    {
        Bind(0);
    }

    /**
     * @brief Binds this buffer.
     * 
     */
    inline void bind() const
    {
        BufferGL<TARGET>::Bind(m_RenderID);
    }

    /**
     * @brief Unbinds this buffer (unbinds any buffer of target TARGET).
     * 
     */
    inline void unbind() const
    {
        Unbind();
    }

    /**
     * @brief Get the Render Id object
     * 
     * @return constexpr GLuint The render id
     */
    inline constexpr GLuint getRenderId() const { return m_RenderID; }

    /** 
     * @return true if the buffer is valid (was created)
     * @return false if the buffer is not valid (was not created or has been deleted)
     */
    inline constexpr bool isValid() const { return getRenderId() != 0; }

private:
    GLuint m_RenderID = 0;
};

using VertexBufferGL = BufferGL<GL_ARRAY_BUFFER>;
using IndexBufferGL = BufferGL<GL_ELEMENT_ARRAY_BUFFER>;

} // namespace vrm
