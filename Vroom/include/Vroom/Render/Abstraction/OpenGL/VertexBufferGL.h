#pragma once

#include "Vroom/Render/Abstraction/OpenGL/BufferGL.h"

namespace vrm
{

class VertexBufferGL : public BufferGL<GL_ARRAY_BUFFER>
{
public:

    /**
     * @brief Construct a new VertexBufferGL object.
     * 
     */
    inline constexpr VertexBufferGL() = default;

    /**
     * @brief Copy constructor. Copies the buffer data to a new buffer.
     * 
     * @param other The other buffer
     */
    inline explicit VertexBufferGL(const VertexBufferGL& other) = default;

    /**
     * @brief Move constructor. Moves the buffer data to a new buffer.
     * 
     * @param other The other buffer
     */
    inline explicit VertexBufferGL(VertexBufferGL&& other) = default;

    /**
     * @brief Destroy the VertexBufferGL object.
     * 
     */
    inline ~VertexBufferGL() noexcept = default;

    /**
     * @brief Copy assignment operator. Copies the buffer data to a new buffer.
     * 
     * @param other The other buffer
     * @return VertexBufferGL& This buffer
     */
    inline VertexBufferGL& operator=(const VertexBufferGL& other) = default;

private:
};

} // namespace vrm
