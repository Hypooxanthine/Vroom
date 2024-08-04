#pragma once

#include <glm/glm.hpp>

#include "Vroom/Render/Abstraction/Texture2D.h"
#include "Vroom/Render/Abstraction/RenderBuffer.h"

namespace vrm
{

class FrameBuffer
{
public:
    struct Specification
    {
        bool onScreen;
        int width, height;
        bool useBlending, useDepthTest;
        glm::vec4 clearColor;
    };
public:
    FrameBuffer();
    FrameBuffer(const Specification& spec);
    ~FrameBuffer();

    void bind() const;
    void unbind() const;

    void create(const Specification& spec);
    void reset();

    inline unsigned int getRendererID() const { return m_RendererID; }
    
    inline const Specification& getSpecification() const { return m_Specification; }

    inline const Texture2D& getTexture() const { return m_Texture; }
    inline const RenderBuffer& getRenderBuffer() const { return m_RenderBuffer; }

    void setOnScreenRender(bool onScreen);

    void clearColorBuffer() const;

private:
    unsigned int m_RendererID = 0;
    Specification m_Specification;

    Texture2D m_Texture;
    RenderBuffer m_RenderBuffer;
};

} // namespace vrm