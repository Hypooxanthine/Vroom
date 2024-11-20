#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm
{
	
class ComputeShader
{
public:
    ComputeShader();
    ~ComputeShader();

    bool loadFromFile(const std::string& filePath);
    bool loadFromSource(const std::string& source);
    void unload();
    void bind() const;
    void dispatch(unsigned int x, unsigned int y, unsigned int z) const;
    void dispatchCustomBarrier(unsigned int x, unsigned int y, unsigned int z, unsigned int barrier) const;

    inline void setMemoryBarrier(unsigned int barrier) { m_MemoryBarrier = barrier; }
    inline unsigned int getMemoryBarrier() const { return m_MemoryBarrier; }

	/**
	 * @brief Sends int data to shader.
	 * @param name Uniform name.
	 * @param value Data to send.
	 */
	void setUniform1i(const std::string& name, int value) const;

	/**
	 * @brief Sends int data to shader.
	 * @param name Uniform name.
	 * @param count Number of elements in the array.
	 * @param value Data to send.
	 */
	void setUniform1iv(const std::string& name, int count, const int* value) const;

	/**
	 * @brief Sends unsigned int data to shader.
	 * @param name Uniform name.
	 * @param value Data to send.
	 */
	void setUniform1ui(const std::string& name, unsigned int value) const;

	/**
	 * @brief Sends unsigned int data to shader.
	 * @param name Uniform name.
	 * @param v0 First unsigned int vector component to send.
	 * @param v1 Second unsigned int vector component to send.
	 */
	void setUniform2ui(const std::string& name, unsigned int v0, unsigned int v1) const;

	/**
	 * @brief Sends float data to shader.
	 * @param name Uniform name.
	 * @param value Data to send.
	 */
	void setUniform1f(const std::string& name, float value) const;

	/**
	 * @brief Sends float data to shader.
	 * @param name Uniform name.
	 * @param v0 First float vector component to send.
	 * @param v1 Second float vector component to send.
	 */
	void setUniform2f(const std::string& name, float v0, float v1) const;

	/**
	 * @brief Sends float data to shader.
	 * @param name Uniform name.
	 * @param v0 First float vector component to send.
	 * @param v1 Second float vector component to send.
	 * @param v2 Third float vector component to send.
	 */
	void setUniform3f(const std::string& name, float v0, float v1, float v2) const;

	/**
	 * @brief Sends vec3f data to shader.
	 * @param name Uniform name.
	 * @param vec Data to send.
	 */
	void setUniform3f(const std::string& name, const glm::vec3& vec) const;

	/**
	 * @brief Sends float data to shader.
	 * @param name Uniform name.
	 * @param v0 First float vector component to send.
	 * @param v1 Second float vector component to send.
	 * @param v2 Third float vector component to send.
	 * @param v3 Fourth float vector component to send.
	 */
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;

	/**
	 * @brief Sends mat4f data to shader.
	 * @param name Uniform name.
	 * @param mat Data to send.
	 */
	void setUniformMat4f(const std::string& name, const glm::mat4& mat) const;

private:
	int getUniformLocation(const std::string& name) const;
    
private:
	unsigned int m_RendererID = 0;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
    unsigned int m_MemoryBarrier = GL_ALL_BARRIER_BITS;
    
};

} // namespace vrm