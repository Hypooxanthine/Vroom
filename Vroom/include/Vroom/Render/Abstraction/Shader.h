#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

namespace vrm
{

class Shader
{
public:
	/**
	 * @brief Constructs a Shader object.
	 */
	Shader();

	/**
	 * @brief Releases GPU memory.
	 */
	virtual ~Shader();
	
	/**
	 * @brief Loads a shader from vertex, fragment & geometry shader paths. 
	 * 
	 * @param vertexShaderPath Vertex shader path.
	 * @param fragmentShaderPath Fragment shader path.
	 * @param geometryShaderPath Geometry shader path.
	 * @return true If loaded successfuly.
	 * @return false If at least one shader couldn't be loaded.
	 */
	bool loadFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath);

	/**
	 * @brief Loads a shader from vertex & fragment shader paths.
	 * @param vertexShaderPath Vertex shader path.
	 * @param fragmentShaderPath Fragment shader path.
	 * @return true If loaded successfuly.
	 * @return false If at least one shader couldn't be loaded.
	 */
	bool loadFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	bool loadFromSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& geometryShaderSource);

	bool loadFromSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

	/**
	 * @brief Unloads the shader and releases GPU memory.
	 */
	void unload();

	/**
	 * @brief Binds this shader.
	 */
	void bind() const;

	/**
	 * @brief Unbinds this shader.
	 */
	void unbind() const;

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

	/**
	 * @brief Gets OpenGL ID from this shader.
	 * @return OpenGL ID.
	 */
	inline unsigned int getID() const { return m_RendererID; }

private:
	unsigned int compileShader(unsigned int type, const std::string& source);
	
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	int getUniformLocation(const std::string& name) const;
	
private:
	unsigned int m_RendererID = 0;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};

} // namespace vrm