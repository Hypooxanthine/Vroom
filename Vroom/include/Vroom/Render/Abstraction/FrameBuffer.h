#pragma once

#include "Vroom/Render/Abstraction/Texture2D.h"
#include "Vroom/Render/Abstraction/RenderBuffer.h"

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void bind() const;
    void unbind() const;

    void create(int width, int height);

    inline unsigned int getRendererID() const { return m_RendererID; }

    inline int getWidth() const { return m_Width; }
    inline int getHeight() const { return m_Height; }

    inline const Texture2D& getTexture() const { return m_Texture; }
    inline const RenderBuffer& getRenderBuffer() const { return m_RenderBuffer; }

private:
    unsigned int m_RendererID;
    int m_Width, m_Height;

    Texture2D m_Texture;
    RenderBuffer m_RenderBuffer;
};