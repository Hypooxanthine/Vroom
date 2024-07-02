#include "Vroom/Asset/Parsing/MaterialParsing.h"

#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "Vroom/Core/Assert.h"

namespace vrm
{

MaterialParsing::ParsingResults MaterialParsing::Parse(const std::string& filePath)
{
    // Getting material data
    std::ifstream file(filePath);
    VRM_ASSERT_MSG(file.is_open(), "Failed to open material file: {}", filePath);

    auto parameters = getMaterialParameters(file);
    file.close();

    // Reading vertex shader
    file.open(parameters.vertex);
    VRM_ASSERT_MSG(file.is_open(), "Failed to open vertex shader file: {}", parameters.vertex);

    ParsingResults output;

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
        else if (line == "#include Sampler2DUniform")
        {
            auto size = parameters.textures.size();
            if (size > 0)
                fragSS << "uniform sampler2D u_Texture[" << parameters.textures.size() << "];\n";
        }
        else
        {
            fragSS << line << '\n';
        }
    }
    
    output.fragment = fragSS.str();

    output.texturePaths = std::move(parameters.textures);

    return output;
}

const MaterialParsing::MaterialParameters MaterialParsing::getMaterialParameters(std::ifstream& file)
{

    static const std::unordered_set<std::string> allowedParameters = {
        "vertex", "shading-model", "prefrag", "postfrag",
        "frag-texture-slot-0", "frag-texture-slot-1", "frag-texture-slot-2", "frag-texture-slot-3", "frag-texture-slot-4", "frag-texture-slot-5", "frag-texture-slot-6", "frag-texture-slot-7"
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

    MaterialParameters out;

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

        if (token.starts_with("frag-texture-slot"))
        {
            size_t lastDash = token.find_last_of('-');
            size_t slot = std::stoi(token.substr(lastDash + 1));
            VRM_ASSERT_MSG(out.textures.size() == slot, "Invalid texture slot: {}. You need to specify texture slots from 0 to 7, one by one.", slot);
            out.textures.push_back(value);

            LOG_TRACE("Texture slot {}: {}", slot, value);
        }

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

    out.postfrag = parameters["postfrag"];
    out.prefrag = parameters["prefrag"];
    out.shadingModel = parameters["shading-model"];
    out.vertex = parameters["vertex"];
    
    return out;
}
    

} // namespace vrm
