#include "Vroom/Asset/AssetData/ShaderData.h"

#include <unordered_map>
#include <format>
#include <fstream>

#include "Vroom/Core/Log.h"
#include "Vroom/Core/Assert.h"

using namespace vrm;
using namespace std::string_literals;

void ShaderData::CombinedShader::dump(const std::string& path) const
{
  std::ofstream ofs;
  ofs.open(path);
  VRM_CHECK_RET(ofs.is_open());

  ofs << sourceCode;

  ofs.close();
}

void ShaderData::ShaderProperties::absorb(const ShaderProperties& other)
{
  enable = enable || other.enable;
  if (version.empty())
  {
    version = other.version;
  }
  
  extensions.insert(extensions.end(), other.extensions.begin(), other.extensions.end());
  earlySources.insert(earlySources.end(), other.earlySources.begin(), other.earlySources.end());
  sources.insert(sources.end(), other.sources.begin(), other.sources.end());
  defines.insert(defines.end(), other.defines.begin(), other.defines.end());
  vertexAttributes.insert(vertexAttributes.end(), other.vertexAttributes.begin(), other.vertexAttributes.end());
  uniforms.insert(uniforms.end(), other.uniforms.begin(), other.uniforms.end());
  uniformBuffers.insert(uniformBuffers.end(), other.uniformBuffers.begin(), other.uniformBuffers.end());
  storageBuffers.insert(storageBuffers.end(), other.storageBuffers.begin(), other.storageBuffers.end());
}

void ShaderData::absorb(const ShaderData& other)
{
  m_varyings.insert(m_varyings.end(), other.m_varyings.begin(), other.m_varyings.end());
  m_globalProperties.absorb(other.m_globalProperties);

  for (uint8_t i = 0; i < static_cast<uint8_t>(EShaderType::eCount); ++i)
  {
    auto type = static_cast<EShaderType>(i);
    getProps(type).absorb(other.getProps(type));
  }
}

void ShaderData::setShaderEnabled(const EShaderType& shader, const bool enable)
{
  getProps(shader).enable = enable;
}

void ShaderData::setVersion(const EShaderType& shader, const std::string& version)
{
  getProps(shader).version = version;
}

void ShaderData::addExtension(const EShaderType& shader, const Extension& ext)
{
  getProps(shader).extensions.emplace_back(ext);
}

void ShaderData::addDefine(const EShaderType& shader, const Define& define)
{
  getProps(shader).defines.emplace_back(define);
}

void ShaderData::addVertexAttribute(const VertexAttribute& attrib)
{
  getProps(EShaderType::eVertex).vertexAttributes.emplace_back(attrib);
}

void ShaderData::addVarying(const Varying& varying)
{
  m_varyings.emplace_back(varying);
}

void ShaderData::addUniform(const EShaderType& shader, const Uniform& uniform)
{
  getProps(shader).uniforms.emplace_back(uniform);
}

void ShaderData::addUniformBuffer(const EShaderType& shader, const UniformBuffer& uniformBuffer)
{
  getProps(shader).uniformBuffers.emplace_back(uniformBuffer);
}

void ShaderData::addStorageBuffer(const EShaderType& shader, const StorageBuffer& storageBuffer)
{
  getProps(shader).storageBuffers.emplace_back(storageBuffer);
}

void ShaderData::addEarlySource(const EShaderType& shader, const SourceElement& source)
{
  getProps(shader).earlySources.emplace_back(source);
}

void ShaderData::addSource(const EShaderType& shader, const SourceElement& source)
{
  getProps(shader).sources.emplace_back(source);
}

bool ShaderData::validate() const
{
  /// @todo Implement validation
  return false;
}

ShaderData::FullShader ShaderData::combine() const
{
  FullShader out = {};
  
  for (uint8_t i = 0; i < static_cast<uint8_t>(EShaderType::eCount); ++i)
  {
    const EShaderType shader = static_cast<EShaderType>(i);
    if (getProps(shader).enable)
    {
      out[shader] = combine(m_globalProperties, getProps(shader), shader, m_varyings);
    }
  }

  return out;
}

ShaderData::ShaderProperties& ShaderData::getProps(const EShaderType& shader)
{
  if (shader == EShaderType::eAll)
    return m_globalProperties;

  return m_shaderProps.at(static_cast<uint8_t>(shader));
}

const ShaderData::ShaderProperties& ShaderData::getProps(const EShaderType& shader) const
{
  return const_cast<ShaderData*>(this)->getProps(shader);
}

std::string ShaderData::combine(const ShaderProperties& global, const ShaderProperties& shader, const EShaderType& shaderType, const std::vector<Varying>& varyings) const
{
  std::string out;
  
  out += combineVersion(global.version, shader.version);
  out += combineDefines(global.defines, shader.defines);
  out += combineExtensions(global.extensions, shader.extensions);
  if (shaderType == EShaderType::eVertex)
  {
    out += combineVertexAttributes(shader.vertexAttributes);
  }
  out += combineSources(global.earlySources, shader.earlySources);
  if (shaderType == EShaderType::eVertex)
  {
    out += combineVaryings("out", varyings);
  }
  else if (shaderType == EShaderType::eFragment)
  {
    out += combineVaryings("in", varyings);
  }
  out += combineUniforms(global.uniforms, shader.uniforms);
  out += combineUniformBuffers(global.uniformBuffers, shader.uniformBuffers);
  out += combineStorageBuffers(global.storageBuffers, shader.storageBuffers);
  out += combineSources(global.sources, shader.sources);

  return out;
}

std::string ShaderData::combineVersion(const std::string& global, const std::string& shader) const
{
  std::string out = "#version ";
  if (shader.empty())
  {
    if (global.empty())
    {
      out += "<ERROR: No version specified>";
    }
    else
    {
      out += global;
    }
  }
  else
  {
    out += shader;
  }

  out += "\n\n";

  return out;
}

std::string ShaderData::combineExtensions(const std::vector<Extension>& global, const std::vector<Extension>& shader) const
{
  std::string out = "// Extensions\n";

  for (const auto& ext : global)
  {
    out += applyEnableIf(std::format("#extension {} : {}\n", ext.name, toString(ext.behaviour)), ext.enableIf);
  }

  for (const auto& ext : shader)
  {
    out += applyEnableIf(std::format("#extension {} : {}\n", ext.name, toString(ext.behaviour)), ext.enableIf);
  }

  out += "\n";

  return out;
}

std::string ShaderData::combineDefines(const std::vector<Define>& global, const std::vector<Define>& shader) const
{
  std::string out = "// Defines\n";

  for (const auto& def : global)
  {
    if (def.value.empty())
    {
      out += std::format("#define {}\n", def.name);
    }
    else
    {
      out += std::format("#define {} {}\n", def.name, def.value);
    }
  }

  for (const auto& def : shader)
  {
    if (def.value.empty())
    {
      out += std::format("#define {}\n", def.name);
    }
    else
    {
      out += std::format("#define {} {}\n", def.name, def.value);
    }
  }

  out += "\n";

  return out;
}

std::string ShaderData::combineVertexAttributes(const std::vector<VertexAttribute>& shader) const
{
  std::string out = "// Vertex attributes\n";

  for (const auto& attrib : shader)
  {
    std::string line;
    if (!attrib.layout.empty())
      line += std::format("layout({}) ", attrib.layout);
    
    line += std::format("in {} {};\n", attrib.type, attrib.name);

    out += applyEnableIf(line, attrib.enableIf);
  }

  return out;
}

std::string ShaderData::combineVaryings(const std::string& prefix, const std::vector<Varying>& varyings) const
{
  std::string out = "// Varyings\n";

  for (const auto& var : varyings)
  {
    std::string line = std::format("{} {} {};\n", prefix, var.type, var.name);
    out += applyEnableIf(line, var.enableIf);
  }

  out += "\n";

  return out;
}

std::string ShaderData::combineUniforms(const std::vector<Uniform>& global, const std::vector<Uniform>& shader) const
{
  std::string out = "// Uniforms\n";

  for (const auto& uni : global)
  {
    std::string line = std::format("uniform {} {};\n", uni.type, uni.name);
    out += applyEnableIf(line, uni.enableIf);
  }

  for (const auto& uni : shader)
  {
    std::string line = std::format("uniform {} {};\n", uni.type, uni.name);
    out += applyEnableIf(line, uni.enableIf);
  }

  out += "\n";

  return out;
}

std::string ShaderData::combineUniformBuffers(const std::vector<UniformBuffer>& global, const std::vector<UniformBuffer>& shader) const
{
  std::string out = "// Uniform Buffers\n";

  for (const auto& ubo : global)
  {
    std::string uboCode = std::format("layout({}) uniform {} {{\n", ubo.layout, ubo.name);

    for (const auto& var : ubo.variables)
    {
      uboCode += applyEnableIf(std::format("  {} {}_{};\n", var.type, ubo.name, var.name), var.enableIf);
    }

    uboCode += "};\n";

    out += applyEnableIf(uboCode, ubo.enableIf);
  }

  for (const auto& ubo : shader)
  {
    std::string uboCode = std::format("layout({}) uniform {} {{\n", ubo.layout, ubo.name);

    for (const auto& var : ubo.variables)
    {
      uboCode += applyEnableIf(std::format("  {} {}_{};\n", var.type, ubo.name, var.name), var.enableIf);
    }

    uboCode += "};\n";

    out += applyEnableIf(uboCode, ubo.enableIf);
  }

  out += "\n";

  return out;
}

std::string ShaderData::combineStorageBuffers(const std::vector<StorageBuffer>& global, const std::vector<StorageBuffer>& shader) const
{
  std::string out = "// Storage Buffers\n";

  for (const auto& ssbo : global)
  {
    std::string ssboCode = std::format("layout({})", ssbo.layout);

    for (const auto& qual : ssbo.memoryQualifiers)
    {
      ssboCode += " " + toString(qual);
    }

    ssboCode += std::format(" buffer {} {{\n", ssbo.name);

    for (const auto& var : ssbo.variables)
    {
      std::string varCode = "  ";
      
      for (const auto& qual : var.memoryQualifiers)
      {
        varCode += toString(qual) + " ";
      }

      varCode += std::format("{} {}_{};\n", var.type, ssbo.name, var.name);

      ssboCode += applyEnableIf(varCode, var.enableIf);
    }

    ssboCode += "};\n";

    out += applyEnableIf(ssboCode, ssbo.enableIf);
  }

  for (const auto& ssbo : shader)
  {
    std::string ssboCode = std::format("layout({})", ssbo.layout);

    for (const auto& qual : ssbo.memoryQualifiers)
    {
      ssboCode += " " + toString(qual);
    }

    ssboCode += std::format(" buffer {} {{\n", ssbo.name);

    for (const auto& var : ssbo.variables)
    {
      std::string varCode = "  ";
      
      for (const auto& qual : var.memoryQualifiers)
      {
        varCode += toString(qual) + " ";
      }

      varCode += std::format("{} {}_{};\n", var.type, ssbo.name, var.name);

      ssboCode += applyEnableIf(varCode, var.enableIf);
    }

    ssboCode += "};\n";

    out += applyEnableIf(ssboCode, ssbo.enableIf);
  }

  out += "\n";

  return out;
}

std::string ShaderData::combineSources(const std::vector<SourceElement>& global, const std::vector<SourceElement>& shader) const
{
  std::string out = "// Sources\n\n";

  for (const auto& src : global)
  {
    out += "//--------------------------------------------------\n";
    out += "// Sub-shader: " + src.filePath + "\n";
    out += "// Included in all shaders of this program\n";
    out += "//--------------------------------------------------\n";
    out += applyEnableIf(std::format("#line 1\n{}\n", src.content), src.enableIf);
    out += "\n\n";
  }

  for (const auto& src : shader)
  {
    out += "//--------------------------------------------------\n";
    out += "// Sub-shader: " + src.filePath + "\n";
    out += "// Only included in this shader\n";
    out += "//--------------------------------------------------\n";
    out += applyEnableIf(std::format("#line 1\n{}\n", src.content), src.enableIf);
    out += "\n\n";
  }

  return out;
}

std::string ShaderData::applyEnableIf(const std::string& source, const std::string& enableIf) const
{
  if (enableIf.empty())
    return source;

  return std::format("#if defined {}\n{}#endif // defined {}\n", enableIf, source, enableIf);
}

std::string ShaderData::toString(const ShaderData::EMemoryQualifier& qualifier) const
{
  switch (qualifier)
  {
  case vrm::ShaderData::EMemoryQualifier::eCoherent: return "coherent";
  case vrm::ShaderData::EMemoryQualifier::eVolatile: return "volatile";
  case vrm::ShaderData::EMemoryQualifier::eRestrict: return "restrict";
  case vrm::ShaderData::EMemoryQualifier::eReadOnly: return "readonly";
  case vrm::ShaderData::EMemoryQualifier::eWriteOnly: return "writeonly";
  default: return "<ERROR: Unknown memory qualifier>";
  }
}

std::string ShaderData::toString(const ShaderData::Varying::EQualifier& qualifier) const
{
  switch (qualifier)
  {
  case vrm::ShaderData::Varying::EQualifier::eFlat: return "flat";
  case vrm::ShaderData::Varying::EQualifier::eSmooth: return "smooth";
  case vrm::ShaderData::Varying::EQualifier::eNoPerspective: return "noperspective";
  default: return "<ERROR: Unknown qualifier>";
  }
}

std::string ShaderData::toString(const ShaderData::Extension::EBehaviour& behaviour) const
{
  switch (behaviour)
  {
  case vrm::ShaderData::Extension::EBehaviour::eRequire: return "require";
  case vrm::ShaderData::Extension::EBehaviour::eEnable: return "enable";
  case vrm::ShaderData::Extension::EBehaviour::eWarn: return "warn";
  case vrm::ShaderData::Extension::EBehaviour::eDisable: return "disable";
  default: return "<ERROR: Unknown behaviour>";
  }
}
