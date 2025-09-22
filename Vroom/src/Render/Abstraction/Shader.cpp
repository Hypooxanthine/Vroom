#include "Vroom/Render/Abstraction/Shader.h"
#include <format>

#include "Vroom/Core/Log.h"
#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/Texture.h"

using namespace vrm;
using namespace vrm::gl;

Shader::Shader() {}

Shader::~Shader() { unload(); }

glm::uvec3 Shader::ComputeDispatchSize(const glm::uvec3& localSize,
                                       const glm::uvec3& desiredMinInvocations)
{
  return (desiredMinInvocations + localSize - glm::uvec3(1)) / localSize;
}

bool Shader::addShaderStage(const EShaderType& type, const GLString& source,
                            bool checkErrors)
{
  if (m_RendererID == 0) { m_RendererID = glCreateProgram(); }

  const static std::unordered_map<EShaderType, GLenum> table = {
    {   EShaderType::eVertex,   GL_VERTEX_SHADER },
    { EShaderType::eGeometry, GL_GEOMETRY_SHADER },
    { EShaderType::eFragment, GL_FRAGMENT_SHADER },
    {  EShaderType::eCompute,  GL_COMPUTE_SHADER }
  };

  VRM_CHECK_RET_FALSE_MSG(table.contains(type), "Unsupported shader type {}",
                          static_cast<uint8_t>(type));
  GLenum glType = table.at(type);
  GLuint shader = CompileShader(glType, source, checkErrors);

  VRM_CHECK_RET_FALSE_MSG(shader != 0, "Shader compile error");

  GLCall(glAttachShader(m_RendererID, shader));
  m_attachedShaders.emplace(shader);

  return true;
}

GLuint Shader::CompileShader(GLenum type, const GLString& source,
                             bool recordErrors)
{
  GLCall(GLuint id = glCreateShader(type));
  const GLchar* src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE)
  {
    if (recordErrors)
    {
      int length;
      GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
      GLString str(length, ' ');
      GLCall(glGetShaderInfoLog(id, length, &length, str.data()));
      m_errorRecord += str;
      m_errorRecord += "\n";
    }

    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

bool Shader::validate(bool checkErrors)
{
  GLCall(glLinkProgram(m_RendererID));

  GLint linkStatus;
  GLCall(glGetProgramiv(m_RendererID, GL_LINK_STATUS, &linkStatus));
  if (linkStatus == GL_FALSE)
  {
    if (checkErrors)
    {
      GLint maxLength = 0;
      GLCall(glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength));

      std::basic_string<GLchar> s(maxLength, ' ');

      GLCall(
        glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, s.data()));
      m_errorRecord += std::format("[Link status {:x}]\n", linkStatus);
      m_errorRecord += s;
      m_errorRecord += " ";
    }

    GLCall(glDeleteProgram(m_RendererID));
    m_RendererID = 0;
  }

  if (m_RendererID)
  {
    GLCall(glValidateProgram(m_RendererID));
    GLint validateStatus;
    GLCall(glGetProgramiv(m_RendererID, GL_VALIDATE_STATUS, &validateStatus));
    if (validateStatus == GL_FALSE)
    {
      if (checkErrors)
      {
        GLint maxLength = 0;
        GLCall(glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength));

        std::basic_string<GLchar> s(maxLength, ' ');

        GLCall(
          glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, s.data()));
        m_errorRecord +=
          std::format("[Validate status {:x}]\n", validateStatus);
        m_errorRecord += s;
        m_errorRecord += " ";
      }

      GLCall(glDeleteProgram(m_RendererID));
      m_RendererID = 0;
    }
  }

  for (const auto& shader : m_attachedShaders)
  {
    GLCall(glDeleteShader(shader));
  }
  m_attachedShaders.clear();

  return m_RendererID != 0;
}

Shader::GLString Shader::getError()
{
  auto error    = std::move(m_errorRecord);
  m_errorRecord = "";
  return std::move(error);
}

void Shader::unload()
{
  if (m_RendererID != 0)
  {
    GLCall_nothrow(glDeleteProgram(m_RendererID));
    m_RendererID = 0;
    m_attachedShaders.clear();
    m_UniformLocationCache.clear();
    m_ssboIndexCache.clear();
    m_nextSsboLoc = 0;
  }
}

void Shader::bind() const { GLCall(glUseProgram(m_RendererID)); }

void Shader::unbind() const { GLCall(glUseProgram(0)); }

void Shader::setUniform1i(const GLString& name, int value) const
{
  GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1iv(const GLString& name, int count,
                           const int* value) const
{
  GLCall(glUniform1iv(getUniformLocation(name), count, value));
}

void Shader::setUniform1ui(const GLString& name, unsigned int value) const
{
  GLCall(glUniform1ui(getUniformLocation(name), value));
}

void Shader::setUniform2ui(const GLString& name, unsigned int v0,
                           unsigned int v1) const
{
  GLCall(glUniform2ui(getUniformLocation(name), v0, v1));
}

void Shader::setUniform1f(const GLString& name, float value) const
{
  GLCall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform2f(const GLString& name, float v0, float v1) const
{
  GLCall(glUniform2f(getUniformLocation(name), v0, v1));
}

void Shader::setUniform3f(const GLString& name, float v0, float v1,
                          float v2) const
{
  GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
}

void Shader::setUniform3f(const GLString& name, const glm::vec3& vec) const
{
  setUniform3f(name, vec.x, vec.y, vec.z);
}

void Shader::setUniform4f(const GLString& name, float v0, float v1, float v2,
                          float v3) const
{
  GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const GLString& name, const glm::mat4& mat) const
{
  GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

int Shader::getUniformLocation(const GLString& name) const
{
  if (m_UniformLocationCache.contains(name))
    return m_UniformLocationCache.at(name);

  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

  m_UniformLocationCache[name] = location;

  return location;
}

GLuint Shader::getStorageBufferLocation(const GLString& name) const
{
  if (!m_ssboIndexCache.contains(name))
  {
    SsboIndexLoc indexLoc;
    indexLoc.index = glGetProgramResourceIndex(
      m_RendererID, GL_SHADER_STORAGE_BLOCK, name.c_str());
    if (indexLoc.index != GL_INVALID_INDEX)
    {
      indexLoc.loc = m_nextSsboLoc++;
      glShaderStorageBlockBinding(m_RendererID, indexLoc.index, indexLoc.loc);
    }
    else
    {
      indexLoc.loc = -1;
      VRM_LOG_ERROR("Could not find StorageBuffer \"{}\" in shader. Maybe it "
                    "is dead code ?",
                    name);
    }

    m_ssboIndexCache[name] = indexLoc;
  }

  return m_ssboIndexCache.at(name).loc;
}

void Shader::setTexture(const GLString& name, const Texture& texture,
                        GLuint slot) const
{
  glActiveTexture(GL_TEXTURE0 + slot);
  texture.bind();
  setUniform1i(name, slot);
}

void Shader::setStorageBuffer(const GLString&   name,
                              const gl::Buffer& ssbo) const
{
  GLuint loc = getStorageBufferLocation(name);
  if (loc != -1)
  {
    Buffer::Bind(ssbo, GL_SHADER_STORAGE_BUFFER);
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, loc, ssbo.getRenderId()));
  }
}

void Shader::setStorageBuffer(const GLString& name, const gl::Buffer& ssbo,
                              GLintptr offset, GLintptr size) const
{
  GLuint loc = getStorageBufferLocation(name);
  if (loc != -1)
  {
    Buffer::Bind(ssbo, GL_SHADER_STORAGE_BUFFER);
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, loc, ssbo.getRenderId()));
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, loc, ssbo.getRenderId(), offset,
                      size);
  }
}
