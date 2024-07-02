#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace vrm
{

class MaterialParsing
{
public:
    struct ParsingResults
    {
        std::string vertex;
        std::string fragment;
        std::vector<std::string> texturePaths;
    };

public:
    MaterialParsing() = delete;
    
    static ParsingResults Parse(const std::string& filePath);

private:


private:
    struct MaterialParameters
    {
        std::string vertex;
        std::string shadingModel;
        std::string prefrag;
        std::string postfrag;
        std::vector<std::string> textures;
    };

    static const MaterialParameters getMaterialParameters(std::ifstream& file);
};

} // namespace vrm
