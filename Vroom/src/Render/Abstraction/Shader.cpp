#include "Vroom/Render/Abstraction/Shader.h"

#include <iostream>
#include <fstream>

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Core/Log.h"

namespace vrm
{

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

Shader::Shader()
{
}

Shader::~Shader()
{
    unload();
}

bool Shader::loadFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath)
{
    unload();

    std::string vertexShader = LoadShader(vertexShaderPath);
    std::string fragmentShader = LoadShader(fragmentShaderPath);
    std::string geometryShader = LoadShader(geometryShaderPath);
    
    return loadFromSource(vertexShader, fragmentShader, geometryShader);
}

bool Shader::loadFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    unload();

    std::string vertexShader = LoadShader(vertexShaderPath);
    std::string fragmentShader = LoadShader(fragmentShaderPath);
    
    return loadFromSource(vertexShader, fragmentShader);
}

bool Shader::loadFromSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& geometryShaderSource)
{
    unload();

    m_RendererID = createShader(vertexShaderSource, fragmentShaderSource, geometryShaderSource);
    if (m_RendererID == 0) return false;
    
    return true;
}

bool Shader::loadFromSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
    unload();

    m_RendererID = createShader(vertexShaderSource, fragmentShaderSource);
    if (m_RendererID == 0) return false;
    
    return true;
}

void Shader::unload()
{
    if (m_RendererID != 0)
    {
        GLCall_nothrow(glDeleteProgram(m_RendererID));
        m_RendererID = 0;
    }
}

void Shader::bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int value) const
{
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1iv(const std::string& name, int count, const int* value) const
{
    GLCall(glUniform1iv(getUniformLocation(name), count, value));
}

void Shader::setUniform1ui(const std::string& name, unsigned int value) const
{
    GLCall(glUniform1ui(getUniformLocation(name), value));
}

void Shader::setUniform2ui(const std::string& name, unsigned int v0, unsigned int v1) const
{
    GLCall(glUniform2ui(getUniformLocation(name), v0, v1));
}

void Shader::setUniform1f(const std::string& name, float value) const
{
    GLCall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform2f(const std::string& name, float v0, float v1) const
{
    GLCall(glUniform2f(getUniformLocation(name), v0, v1));
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) const
{
    GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
}

void Shader::setUniform3f(const std::string& name, const glm::vec3& vec) const
{
    setUniform3f(name, vec.x, vec.y, vec.z);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& mat) const
{
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
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
        VRM_LOG_CRITICAL("Failed to compile {} shader: {}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), str);
        GLCall(glDeleteShader(id));

        VRM_LOG_INFO("Shader source:");
        outputSource(source);
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    unsigned int gs = compileShader(GL_GEOMETRY_SHADER, geometryShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, gs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLint linkStatus;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linkStatus));
    if (linkStatus == GL_FALSE) {
        GLint maxLength = 0;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));

        std::vector<GLchar> infoLog(maxLength);
        GLCall(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));

        std::cerr << "Program link error: " << &infoLog[0] << std::endl;

        GLCall(glDeleteProgram(program));
        return 0;
    }

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(gs));
    GLCall(glDeleteShader(fs));

    return program;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int Shader::getUniformLocation(const std::string& name) const
{
    if (m_UniformLocationCache.contains(name))
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    //if (location == -1)
    //    std::cout << "Uniform " << name << " does not exist." << std::endl;
    
    m_UniformLocationCache[name] = location;

    return location;
}

} // namespace vrm