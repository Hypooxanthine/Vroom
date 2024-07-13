#pragma once

#include <string>
#include <unordered_map>

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
    
private:
	unsigned int m_RendererID = 0;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
    
};