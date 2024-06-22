#include "Vroom/Asset/StaticAsset/ShaderAsset.h"

#include <unordered_set>
#include <fstream>

#include "Vroom/Asset/AssetInstance/ShaderInstance.h"
#include "Vroom/Core/Log.h"

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

const Shader& ShaderAsset::getShader() const
{
    return m_Shader;
}

bool ShaderAsset::loadImpl(const std::string& filePath, AssetManager& manager)
{
    static std::unordered_set<std::string> shaderTypes = { "vertex", "fragment", "geometry" };

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        LOG_WARN("Failed to open file: {}", filePath);
        return false;
    }

    std::unordered_map<std::string, std::string> shaderPaths;
    std::string line;
    
    // Layout is
    // vertex <path>
    // fragment <path>
    // ...

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string type;

        if (iss.eof())
            continue;
        
        iss >> type;

        if (!shaderTypes.contains(type))
        {
            LOG_WARN("While parsing shader file: {}", filePath);
            LOG_WARN("Invalid shader type: {}", type);
            return false;
        }

        if (shaderPaths.contains(type))
        {
            LOG_WARN("While parsing shader file: {}", filePath);
            LOG_WARN("Duplicate shader type: {}", type);
            return false;
        }

        if (iss.eof())
        {
            LOG_WARN("While parsing shader file: {}", filePath);
            LOG_WARN("No path specified for shader type: {}", type);
            return false;
        }

        std::string path;
        iss >> path;

        shaderPaths[type] = path;
    }

    if (!shaderPaths.contains("vertex"))
    {
        LOG_WARN("While parsing shader file: {}", filePath);
        LOG_WARN("No vertex shader specified");
        return false;
    }

    if (!shaderPaths.contains("fragment"))
    {
        LOG_WARN("While parsing shader file: {}", filePath);
        LOG_WARN("No fragment shader specified");
        return false;
    }

    bool ok;

    if (shaderPaths.contains("geometry"))
        ok = m_Shader.loadFromFile(shaderPaths["vertex"], shaderPaths["fragment"], shaderPaths["geometry"]);
    else
        ok = m_Shader.loadFromFile(shaderPaths["vertex"], shaderPaths["fragment"]);

    if (!ok)
    {
        LOG_WARN("Failed to load shader from file: {}", filePath);
        return false;
    }

    LOG_INFO("Loaded shader from file: {}", filePath);
    return true;
}

} // namespace vrm