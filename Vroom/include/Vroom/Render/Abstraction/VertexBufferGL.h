#pragma once

#include "Vroom/Render/Abstraction/BufferGL.h"

namespace vrm
{

class VertexBufferGL : public BufferGL<GL_ARRAY_BUFFER>
{
public:

    inline constexpr VertexBufferGL()
    {}

    inline explicit VertexBufferGL(const VertexBufferGL& other)
        : BufferGL(other)
    {}

    inline explicit VertexBufferGL(VertexBufferGL&& other)
        : BufferGL(std::move(other))
    {}

    inline ~VertexBufferGL() noexcept
    {}

    inline VertexBufferGL& operator=(const VertexBufferGL& other)
    {
        if (this == &other) return *this;

        BufferGL::operator=(other);

        return *this;
    }

private:
};

} // namespace vrm
