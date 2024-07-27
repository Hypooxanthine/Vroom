#include "Vroom/Render/Abstraction/ComputeShader.h"

#include <fstream>

#include "Vroom/Core/Log.h"

static std::string LoadShader(const std::string& path)
{
    std::ifstream ifs(path);

    VRM_ASSERT_MSG(ifs.is_open(), "Failed to open file: {}", path);

    std::string out;
    
    for (std::string line; std::getline(ifs, line);)
        out += (line + '\n');

    return out;
}

static void outputSource(const std::string& source)
{
    std::istringstream iss(source);
    std::string line;
    int lineCount = 1;
    while (std::getline(iss, line))
    {
        VRM_LOG_TRACE("{:04d}: {}", lineCount, line);
        lineCount++;
    }
}

ComputeShader::ComputeShader()
{
}

ComputeShader::~ComputeShader()
{
    unload();
}

bool ComputeShader::loadFromFile(const std::string& filePath)
{
    std::string source = LoadShader(filePath);
    
    return loadFromSource(source);
}

bool ComputeShader::loadFromSource(const std::string& source)
{
    unload();

    unsigned int program = glCreateProgram();
    
    GLCall(unsigned int id = glCreateShader(GL_COMPUTE_SHADER));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        std::string str(length, ' ');
        GLCall(glGetShaderInfoLog(id, length, &length, str.data()));
        VRM_LOG_CRITICAL("Failed to compile compute shader: {}", str);
        GLCall(glDeleteShader(id));

        VRM_LOG_INFO("Shader source:");
        outputSource(source);
        return false;
    }

    GLCall(glAttachShader(program, id));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLint linkStatus;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linkStatus));
    if (linkStatus == GL_FALSE) {
        GLint maxLength = 0;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));

        std::vector<GLchar> infoLog(maxLength);
        GLCall(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));

        VRM_LOG_CRITICAL("Program link error: {}", &infoLog[0]);

        GLCall(glDeleteProgram(program));
        return false;
    }

    GLCall(glDeleteShader(id));

    m_RendererID = program;

    return true;
}

void ComputeShader::unload()
{
    if (m_RendererID != 0)
    {
        GLCall_nothrow(glDeleteProgram(m_RendererID));
        m_RendererID = 0;
    }
}

void ComputeShader::bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void ComputeShader::dispatch(unsigned int x, unsigned int y, unsigned int z) const
{
    dispatchCustomBarrier(x, y, z, m_MemoryBarrier);
}

void ComputeShader::dispatchCustomBarrier(unsigned int x, unsigned int y, unsigned int z, unsigned int barrier) const
{
    bind();
    GLCall(glDispatchCompute(x, y, z));
    GLCall(glMemoryBarrier(barrier));
}

void ComputeShader::setUniform1i(const std::string& name, int value) const
{
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void ComputeShader::setUniform1iv(const std::string& name, int count, const int* value) const
{
    GLCall(glUniform1iv(getUniformLocation(name), count, value));
}

void ComputeShader::setUniform1ui(const std::string& name, unsigned int value) const
{
    GLCall(glUniform1ui(getUniformLocation(name), value));
}

void ComputeShader::setUniform2ui(const std::string& name, unsigned int v0, unsigned int v1) const
{
    GLCall(glUniform2ui(getUniformLocation(name), v0, v1));
}

void ComputeShader::setUniform1f(const std::string& name, float value) const
{
    GLCall(glUniform1f(getUniformLocation(name), value));
}

void ComputeShader::setUniform2f(const std::string& name, float v0, float v1) const
{
    GLCall(glUniform2f(getUniformLocation(name), v0, v1));
}

void ComputeShader::setUniform3f(const std::string& name, float v0, float v1, float v2) const
{
    GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
}

void ComputeShader::setUniform3f(const std::string& name, const glm::vec3& vec) const
{
    setUniform3f(name, vec.x, vec.y, vec.z);
}

void ComputeShader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void ComputeShader::setUniformMat4f(const std::string& name, const glm::mat4& mat) const
{
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

int ComputeShader::getUniformLocation(const std::string& name) const
{
    if (m_UniformLocationCache.contains(name))
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    //if (location == -1)
    //    std::cout << "Uniform " << name << " does not exist." << std::endl;
    
    m_UniformLocationCache[name] = location;

    return location;
}