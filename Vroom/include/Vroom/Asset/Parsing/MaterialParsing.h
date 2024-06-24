#pragma once

#include <fstream>

namespace vrm
{

class MaterialParsing
{
public:
    struct ShadersOutput
    {
        std::string vertex;
        std::string fragment;
    };

public:
    MaterialParsing() = delete;
    
    static ShadersOutput Parse(const std::string& filePath);

private:


private:
    struct MaterialParameters
    {
        std::string vertex;
        std::string shadingModel;
        std::string prefrag;
        std::string postfrag;
    };

    static const MaterialParameters getMaterialParameters(std::ifstream& file);
};

} // namespace vrm
