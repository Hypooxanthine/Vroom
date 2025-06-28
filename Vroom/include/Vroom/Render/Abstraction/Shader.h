#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Vroom/Render/Abstraction/GLCall.h"

#include "glm/glm.hpp"

namespace vrm::gl
{

  class Texture;
  class StorageBufferBase;

  class Shader
  {
  public:
    enum class EShaderType : uint8_t
    {
      eVertex = 0,
      eGeometry,
      eFragment,
      eCompute
    };

    using GLString = std::basic_string<GLchar>;

  public:
    /**
     * @brief Constructs a Shader object.
     */
    Shader();

    /**
     * @brief Releases GPU memory.
     */
    virtual ~Shader();

    bool addShaderStage(const EShaderType &type, const GLString &source, bool checkErrors);
    bool validate(bool checkErrors);

    GLString getError();

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
    void setUniform1i(const GLString &name, int value) const;

    /**
     * @brief Sends int data to shader.
     * @param name Uniform name.
     * @param count Number of elements in the array.
     * @param value Data to send.
     */
    void setUniform1iv(const GLString &name, int count, const int *value) const;

    /**
     * @brief Sends unsigned int data to shader.
     * @param name Uniform name.
     * @param value Data to send.
     */
    void setUniform1ui(const GLString &name, unsigned int value) const;

    /**
     * @brief Sends unsigned int data to shader.
     * @param name Uniform name.
     * @param v0 First unsigned int vector component to send.
     * @param v1 Second unsigned int vector component to send.
     */
    void setUniform2ui(const GLString &name, unsigned int v0, unsigned int v1) const;

    /**
     * @brief Sends float data to shader.
     * @param name Uniform name.
     * @param value Data to send.
     */
    void setUniform1f(const GLString &name, float value) const;

    /**
     * @brief Sends float data to shader.
     * @param name Uniform name.
     * @param v0 First float vector component to send.
     * @param v1 Second float vector component to send.
     */
    void setUniform2f(const GLString &name, float v0, float v1) const;

    inline void setUniform2f(const GLString &name, const glm::vec2&vec ) const { setUniform2f(name, vec.x, vec.y); }

    /**
     * @brief Sends float data to shader.
     * @param name Uniform name.
     * @param v0 First float vector component to send.
     * @param v1 Second float vector component to send.
     * @param v2 Third float vector component to send.
     */
    void setUniform3f(const GLString &name, float v0, float v1, float v2) const;

    /**
     * @brief Sends vec3f data to shader.
     * @param name Uniform name.
     * @param vec Data to send.
     */
    void setUniform3f(const GLString &name, const glm::vec3 &vec) const;

    /**
     * @brief Sends float data to shader.
     * @param name Uniform name.
     * @param v0 First float vector component to send.
     * @param v1 Second float vector component to send.
     * @param v2 Third float vector component to send.
     * @param v3 Fourth float vector component to send.
     */
    void setUniform4f(const GLString &name, float v0, float v1, float v2, float v3) const;

    inline void setUniform4f(const GLString &name, const glm::vec4&vec ) const { setUniform4f(name, vec.x, vec.y, vec.z, vec.w); }

    /**
     * @brief Sends mat4f data to shader.
     * @param name Uniform name.
     * @param mat Data to send.
     */
    void setUniformMat4f(const GLString &name, const glm::mat4 &mat) const;

    void setTexture(const GLString &name, const Texture& texture, GLuint slot) const;

    void setStorageBuffer(const GLString &name, const StorageBufferBase &ssbo) const;

    /**
     * @brief Gets OpenGL ID from this shader.
     * @return OpenGL ID.
     */
    inline unsigned int getID() const { return m_RendererID; }

    int getUniformLocation(const GLString &name) const;

  private:
    GLuint CompileShader(GLenum type, const GLString &source, bool recordErrors);
    GLuint getStorageBufferIndex(const GLString &name) const;

  private:
    unsigned int m_RendererID = 0;
    mutable std::unordered_map<GLString, int> m_UniformLocationCache;
    mutable std::unordered_map<GLString, GLuint> m_ssboIndexCache;

    std::unordered_set<GLuint> m_attachedShaders;
    GLString m_errorRecord;
  };

} // namespace vrm::gl