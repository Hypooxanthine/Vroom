#pragma once

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

class Shader;

namespace vrm
{

class MaterialData
{
public:
    MaterialData();
    ~MaterialData();

    void applyUniforms(const Shader& shader) const;

    void setMat4Uniform(const std::string& name, const glm::mat4& value);
    void setVec4Uniform(const std::string& name, const glm::vec4& value);
    void setVec3Uniform(const std::string& name, const glm::vec3& value);
    void setVec2Uniform(const std::string& name, const glm::vec2& value);
    void setFloatUniform(const std::string& name, float value);
    void setIntUniform(const std::string& name, int value);
    void setBoolUniform(const std::string& name, bool value);
    void setTextureUniform(const std::string& name, unsigned int value);

private:
    std::unordered_map<std::string, glm::mat4> m_Mat4Uniforms;
    std::unordered_map<std::string, glm::vec4> m_Vec4Uniforms;
    std::unordered_map<std::string, glm::vec3> m_Vec3Uniforms;
    std::unordered_map<std::string, glm::vec2> m_Vec2Uniforms;
    std::unordered_map<std::string, float> m_FloatUniforms;
    std::unordered_map<std::string, int> m_IntUniforms;
    std::unordered_map<std::string, bool> m_BoolUniforms;
    std::unordered_map<std::string, unsigned int> m_TextureUniforms;
};

} // namespace vrm
