#include "Vroom/Asset/StaticAsset/ShaderAsset.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Vroom/Core/Assert.h"

#include "Vroom/Asset/AssetManager.h"

namespace vrm
{

ShaderAsset::ShaderAsset()
    : StaticAsset()
{
}

ShaderAsset::~ShaderAsset()
{
}

ShaderInstance ShaderAsset::createInstance()
{
    return ShaderInstance(this);
}

// This will log an error message and return false if the file couldn't be opened (doesn't crash the application)
#define SOFT_ASSERT_MSG(x, ...) if(!(x)) { LOG_ERROR(__VA_ARGS__); return false; }

bool ShaderAsset::loadImpl(const std::string& filePath, AssetManager& manager)
{
    std::ifstream file(filePath);
    SOFT_ASSERT_MSG(file.is_open(), "Failed to open file: {}", filePath);
    
    std::unordered_map<std::string, std::string> shaderPaths;

    std::string line;
    size_t lineNum = 0;
    for (; std::getline(file, line); ++lineNum)
    {        
        std::istringstream stream(line);
        std::string token;

        if (!(stream >> token)) // No token on this line
            continue;

        if (token.starts_with('#')) // Comment
            continue;
        
        if (token == "vertex")
        {
            SOFT_ASSERT_MSG((stream >> token), "Invalid RenderShader asset {} at line {} : Couldn't find vertex shader file path.", filePath, lineNum);

            shaderPaths["vertex"] = token;
            SOFT_ASSERT_MSG(!(stream >> token), "Invalid RenderShader asset {} at line {} : unexpected token \"{}\".", filePath, lineNum, token);
            continue;
        }

        if (token == "fragment")
        {
            SOFT_ASSERT_MSG((stream >> token), "Invalid RenderShader asset {} at line {} : Couldn't find fragment shader file path.", filePath, lineNum);

            stream >> token;
            shaderPaths["fragment"] = token;
            SOFT_ASSERT_MSG(!(stream >> token), "Invalid RenderShader asset {} at line {} : unexpected token \"{}\".", filePath, lineNum, token);
            continue;
        }

        SOFT_ASSERT_MSG(false, "Invalid RenderShader asset {} at line {} : unexpected token \"{}\".", filePath, lineNum, token);
    }

    SOFT_ASSERT_MSG(shaderPaths.contains("vertex"), "Invalid RenderShader asset {} : Couldn't find vertex shader file path.", filePath);
    SOFT_ASSERT_MSG(shaderPaths.contains("fragment"), "Invalid RenderShader asset {} : Couldn't find fragment shader file path.", filePath);

    m_Shader.loadFromFile(shaderPaths["vertex"], shaderPaths["fragment"]);

    return true;
}

} // namespace vrm