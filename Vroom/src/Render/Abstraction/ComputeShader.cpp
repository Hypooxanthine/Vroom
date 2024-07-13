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

void outputSource(const std::string& source)
{
    std::istringstream iss(source);
    std::string line;
    int lineCount = 1;
    while (std::getline(iss, line))
    {
        LOG_TRACE("{:04d}: {}", lineCount, line);
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
        LOG_CRITICAL("Failed to compile compute shader: {}", str);
        GLCall(glDeleteShader(id));

        LOG_INFO("Shader source:");
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

        LOG_CRITICAL("Program link error: {}", &infoLog[0]);

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

void ComputeShader::bind()
{
    GLCall(glUseProgram(m_RendererID));
}

void ComputeShader::dispatch(unsigned int x, unsigned int y, unsigned int z)
{
    bind();
    GLCall(glDispatchCompute(x, y, z));
    GLCall(glMemoryBarrier(m_MemoryBarrier));
}