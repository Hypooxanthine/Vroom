#pragma once

namespace vrm
{

class RenderBuffer
{
public:
    RenderBuffer();
    ~RenderBuffer();

    void bind() const;
    void unbind() const;

    void create(int width, int height);

    inline unsigned int getRendererID() const { return m_RendererID; }

private:
    unsigned int m_RendererID;
    int m_Width, m_Height;
};

} // namespace vrm