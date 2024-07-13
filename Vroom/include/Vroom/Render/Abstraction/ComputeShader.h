#pragma once

#include <string>
#include <unordered_map>

#include "Vroom/Render/Abstraction/GLCall.h"

class ComputeShader
{
public:
    ComputeShader();
    ~ComputeShader();

    bool loadFromFile(const std::string& filePath);
    bool loadFromSource(const std::string& source);
    void unload();
    void bind();
    void dispatch(unsigned int x, unsigned int y, unsigned int z);

    inline void setMemoryBarrier(unsigned int barrier) { m_MemoryBarrier = barrier; }
    inline unsigned int getMemoryBarrier() const { return m_MemoryBarrier; }
    
private:
	unsigned int m_RendererID = 0;
    unsigned int m_MemoryBarrier = GL_ALL_BARRIER_BITS;
    
};