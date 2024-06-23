#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

#include <glm/glm.hpp>
#include <unordered_set>

#include "Vroom/Core/Log.h"

#include "Vroom/Asset/AssetManager.h"

namespace vrm
{

MaterialAsset::MaterialAsset()
    : StaticAsset()
{
}

MaterialAsset::~MaterialAsset()
{
}

MaterialInstance MaterialAsset::createInstance()
{
    return MaterialInstance(this);
}

bool MaterialAsset::loadImpl(const std::string& filePath, AssetManager& manager)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        LOG_WARN("Failed to open file: {}", filePath);
        return false;
    }

    if (file.eof())
    {
        LOG_WARN("Material file is empty: {}", filePath);
        return false;
    }

    // Check for shading model
    std::string token;
    file >> token;

    if (token != "shading-model" || file.eof())
    {
        LOG_WARN("Failed to find shading-model in material file: {}", filePath);
        return false;
    }

    file >> token;

    static const std::unordered_set<std::string> supportedShadingModels = {
        "Phong",
    };
    
    if (supportedShadingModels.find(token) == supportedShadingModels.end())
    {
        LOG_WARN("Unsupported shading model: {}", token);
        LOG_TRACE("Supported shading models are:");
        for (const auto& shadingModel : supportedShadingModels)
            LOG_TRACE("  {}", shadingModel);
        return false;
    }

    // Get the material file as a string

    if (token == "Phong")
    {
        if (loadPhong(file, manager))
        {
            LOG_INFO("Successfully loaded Phong material: {}", filePath);
            return true;
        }
        else return false;
    }

    LOG_WARN("Failed to load material file: {}", filePath);
    return false;

}

bool MaterialAsset::loadPhong(std::ifstream& materialFile, AssetManager& manager)
{
    // Layout of the Phong material file:
    // use-transparency <bool>
    // ambient <r> <g> <b>
    // diffuse <r> <g> <b>
    // specular <r> <g> <b>
    // shininess <float>
    // transparency <float>

    // Skip 1 word on the line
    materialFile.ignore(256, ' ');
    bool useTransparency;
    if (!(materialFile >> useTransparency)) {
        LOG_ERROR("Failed to read use-transparency flag.");
        return false;
    }

    if (useTransparency)
    {
        LOG_WARN("Transparency is not supported yet.");
        return false;
    }

    materialFile.ignore(256, ' ');
    glm::vec3 ambient;
    if (!(materialFile >> ambient.r >> ambient.g >> ambient.b)) {
        LOG_ERROR("Failed to read ambient color.");
        return false;
    }

    materialFile.ignore(256, ' ');
    glm::vec3 diffuse;
    if (!(materialFile >> diffuse.r >> diffuse.g >> diffuse.b)) {
        LOG_ERROR("Failed to read diffuse color.");
        return false;
    }

    materialFile.ignore(256, ' ');
    glm::vec3 specular;
    if (!(materialFile >> specular.r >> specular.g >> specular.b)) {
        LOG_ERROR("Failed to read specular color.");
        return false;
    }

    materialFile.ignore(256, ' ');
    float shininess;
    if (!(materialFile >> shininess)) {
        LOG_ERROR("Failed to read shininess.");
        return false;
    }

    // Create the Phong material
    m_MaterialData.setBoolUniform("u_Ambient.useTexture", false);
    m_MaterialData.setVec3Uniform("u_Ambient.value", ambient);
    m_MaterialData.setBoolUniform("u_Diffuse.useTexture", false);
    m_MaterialData.setVec3Uniform("u_Diffuse.value", diffuse);
    m_MaterialData.setBoolUniform("u_Specular.useTexture", false);
    m_MaterialData.setVec3Uniform("u_Specular.value", specular);
    m_MaterialData.setFloatUniform("u_Shininess", shininess);

    // Setting the right shader
    m_ShaderInstance = manager.getAsset<ShaderAsset>("Resources/Shaders/PhongModelShader.asset");

    // Successfully loaded the Phong material
    return true;
}

} // namespace vrm