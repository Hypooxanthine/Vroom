#include "Vroom/Asset/Parsing/MaterialParsing.h"

#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "Vroom/Core/Assert.h"

namespace vrm
{

MaterialParsing::ShadersOutput MaterialParsing::Parse(const std::string& filePath)
{
    // Getting material data
    std::ifstream file(filePath);
    VRM_ASSERT_MSG(file.is_open(), "Failed to open material file: {}", filePath);

    auto parameters = getMaterialParameters(file);
    file.close();

    // Reading vertex shader
    file.open(parameters.vertex);
    VRM_ASSERT_MSG(file.is_open(), "Failed to open vertex shader file: {}", parameters.vertex);

    ShadersOutput output;

    output.vertex = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Reading fragment shader assembler
    file.open("Resources/Engine/Shader/FragmentShader/FragmentShaderAssembler.glsl");
    VRM_ASSERT_MSG(file.is_open(), "Failed to open fragment shader assembler file: Resources/Engine/Shader/FragmentShader/FragmentShaderAssembler.glsl");

    std::stringstream fragSS;

    std::string line;
    while (std::getline(file, line))
    {
        if (line == "#include PreFragShader")
        {
            std::ifstream includeFile;
            includeFile.open(parameters.prefrag);
            VRM_ASSERT_MSG(includeFile.is_open(), "Failed to open prefrag shader file: {}", parameters.prefrag);
            
            fragSS << includeFile.rdbuf() << '\n';

            includeFile.close();
        }
        else if (line == "#include ShadingModelShader")
        {
            std::ifstream includeFile;
            includeFile.open(parameters.shadingModel);
            VRM_ASSERT_MSG(includeFile.is_open(), "Failed to open shading model shader file: {}", parameters.shadingModel);
            
            fragSS << includeFile.rdbuf() << '\n';

            includeFile.close();
        }
        else if (line == "#include PostFragShader")
        {
            std::ifstream includeFile;
            includeFile.open(parameters.postfrag);
            VRM_ASSERT_MSG(includeFile.is_open(), "Failed to open postfrag shader file: {}", parameters.postfrag);
            
            fragSS << includeFile.rdbuf() << '\n';

            includeFile.close();
        }
        else
        {
            fragSS << line << '\n';
        }
    }
    
    output.fragment = fragSS.str();

    return output;
}

const MaterialParsing::MaterialParameters MaterialParsing::getMaterialParameters(std::ifstream& file)
{

    static const std::unordered_set<std::string> allowedParameters = {
        "vertex", "shading-model", "prefrag", "postfrag"
    };

    static const std::unordered_map<std::string, std::string> defaultParameters = {
        {"vertex"  , "Resources/Engine/Shader/VertexShader/Vertex_Default.glsl"},
        {"prefrag" , "Resources/Engine/Shader/FragmentShader/PreFrag/PreFrag_Phong_Default.glsl"},
        {"postfrag", "Resources/Engine/Shader/FragmentShader/PostFrag/PostFrag_Default.glsl"}
    };

    static const std::unordered_set<std::string> requiredParameters = {
        "shading-model"
    };

    static const std::unordered_map<std::string, std::string> shadingModels = {
        {"Phong", "Resources/Engine/Shader/FragmentShader/ShadingModel/ShadingModelFrag_Phong.glsl"}
    };

    std::unordered_map<std::string, std::string> parameters;

    std::string line;

    while(std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;

        if (!(iss >> token) || token[0] == '#')
            continue;

        VRM_ASSERT_MSG(allowedParameters.contains(token), "Invalid material parameter: {}", token);
        VRM_ASSERT_MSG(!parameters.contains(token), "Duplicate material parameter: {}", token);

        std::string value;
        VRM_ASSERT_MSG(iss >> value, "Missing value for parameter: {}", token);

        parameters[token] = value;

        VRM_ASSERT_MSG(!(iss >> token), "Unexpected token: {}", token);
    }

    for (const auto& [key, value] : defaultParameters)
    {
        if (!parameters.contains(key))
            parameters[key] = value;
    }

    for (const auto& key : requiredParameters)
    {
        VRM_ASSERT_MSG(parameters.contains(key), "Missing required parameter: {}", key);
    }

    VRM_ASSERT_MSG(shadingModels.contains(parameters["shading-model"]), "Invalid shading model: {}", parameters["shading-model"]);

    parameters["shading-model"] = shadingModels.at(parameters["shading-model"]);

    return MaterialParameters{
        .vertex = parameters["vertex"],
        .shadingModel = parameters["shading-model"],
        .prefrag = parameters["prefrag"],
        .postfrag = parameters["postfrag"]
    };

}
    

} // namespace vrm
