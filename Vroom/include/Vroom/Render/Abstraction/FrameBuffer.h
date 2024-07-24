#pragma once

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void bind();
    void unbind();

private:
    unsigned int m_RendererID;
};