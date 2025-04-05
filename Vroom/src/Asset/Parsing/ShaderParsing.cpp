#include "Vroom/Asset/Parsing/ShaderParsing.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Vroom/Core/Assert.h"
#include "Vroom/Asset/Parsing/Json.h"

using namespace vrm;

//--------------------------------------------------
// Json -> ShaderData

#define PARSER_FUNC_TYPE(name, OutType) static bool name(const ShaderData::EShaderType& type, const json& j, OutType& out)
#define PARSER_FUNC(name) PARSER_FUNC_TYPE(name, ShaderData)

static bool TryGetSource(const std::string& filePath, std::string& out)
{
  std::ifstream ifs(filePath, std::ios::in | std::ios::binary);
  CHECK(ifs.is_open(), "Could not open file {}", filePath);

  std::ostringstream ss;
  ss << ifs.rdbuf();
  out = ss.str();

  return true;
}

static bool TryGetSourceData(const json& j, ShaderData::SourceElement& src)
{
  if (j.is_string())
  {
    src.filePath = j.get<std::string>();
  }
  else if (j.is_object())
  {
    CHECK_ATTR_STRING(j, File);
    src.filePath = FileVal;

    IF_HAS_ATTR_STRING(j, EnableIf)
      src.enableIf = EnableIfVal;
  }
  else
    CHECK(false, "Incorrect source element type");

  CHECK(TryGetSource(src.filePath, src.content), "Error while getting source file");

  return true;
}

PARSER_FUNC(ParseSource)
{
  ShaderData::SourceElement src;
  
  TryGetSourceData(j, src);

  out.addSource(type, std::move(src));

  return true;
}

PARSER_FUNC(ParseSources)
{
  CHECK_ARRAY(j);

  for (const auto& j_source : j)
  {
    CHECK(ParseSource(type, j_source, out), "Error while parsing source");
  }

  return true;
}

PARSER_FUNC(ParseEarlySource)
{
  ShaderData::SourceElement src;
  
  TryGetSourceData(j, src);

  out.addEarlySource(type, std::move(src));

  return true;
}

PARSER_FUNC(ParseEarlySources)
{
  CHECK_ARRAY(j);

  for (const auto& j_source : j)
  {
    CHECK(ParseEarlySource(type, j_source, out), "Error while parsing source");
  }

  return true;
}

PARSER_FUNC(ParseExtension)
{
  CHECK_OBJECT(j);

  CHECK_ATTR_STRING(j, Name);
  CHECK_ATTR_STRING(j, Behaviour);

  ShaderData::Extension ext;
  ext.name = NameVal;
  
  static const std::unordered_map<std::string, ShaderData::Extension::EBehaviour> table =
  {
    { "require", ShaderData::Extension::EBehaviour::eRequire },
    { "enable", ShaderData::Extension::EBehaviour::eEnable },
    { "warn", ShaderData::Extension::EBehaviour::eWarn },
    { "disable", ShaderData::Extension::EBehaviour::eDisable },
  };

  CHECK(table.contains(BehaviourVal), "Unexpected behaviour {}", BehaviourVal);
  ext.behaviour = table.at(BehaviourVal);

  IF_HAS_ATTR_STRING(j, EnableIf)
    ext.enableIf = EnableIfVal;

  out.addExtension(type, ext);

  return true;
}

PARSER_FUNC(ParseExtensions)
{
  CHECK_ARRAY(j);

  for (const auto& j_extension : j)
  {
    CHECK(ParseExtension(type, j_extension, out), "Error while parsing extension");
  }

  return true;
}

PARSER_FUNC(ParseDefine)
{
  ShaderData::Define d;

  IF_STRING(j)
    d.name = jVal;
  ELSE
  {
    CHECK_OBJECT(j);
    CHECK_ATTR_STRING(j, Name);
    d.name = NameVal;

    IF_HAS_ATTR_STRING(j, Value)
      d.value = ValueVal;
  }

  out.addDefine(type, d);

  return true;
}

PARSER_FUNC(ParseDefines)
{
  CHECK_ARRAY(j);

  for (const auto& j_define : j)
  {
    CHECK(ParseDefine(type, j_define, out), "Error while parsing define");
  }

  return true;
}

PARSER_FUNC(ParseUniform)
{
  CHECK_OBJECT(j);

  CHECK_ATTR_STRING(j, Name);
  CHECK_ATTR_STRING(j, Type);

  ShaderData::Uniform u;
  u.name = NameVal;
  u.type = TypeVal;

  IF_HAS_ATTR_STRING(j, EnableIf)
    u.enableIf = EnableIfVal;

  out.addUniform(type, u);
  
  return true;
}

PARSER_FUNC(ParseUniforms)
{
  CHECK_ARRAY(j);

  for (const auto& j_uniform : j)
  {
    CHECK(ParseUniform(type, j_uniform, out), "Error while parsing uniform");
  }

  return true;
}

PARSER_FUNC_TYPE(ParseVariable, ShaderData::Variable)
{
  CHECK_OBJECT(j);

  CHECK_ATTR_STRING(j, Name);
  CHECK_ATTR_STRING(j, Type);

  out.name = NameVal;
  out.type = TypeVal;

  IF_HAS_ATTR_STRING(j, EnableIf)
    out.enableIf = EnableIfVal;

  return true;
}

PARSER_FUNC_TYPE(ParseMemoryQualifiers, std::unordered_set<ShaderData::EMemoryQualifier>)
{
  CHECK_ARRAY(j);

  static std::unordered_map<std::string, ShaderData::EMemoryQualifier> table =
  {
    { "coherent", ShaderData::EMemoryQualifier::eCoherent },
    { "restrict", ShaderData::EMemoryQualifier::eRestrict },
    { "volatile", ShaderData::EMemoryQualifier::eVolatile },
    { "readonly", ShaderData::EMemoryQualifier::eReadOnly },
    { "writeonly", ShaderData::EMemoryQualifier::eWriteOnly },
  };

  for (const auto& j_qualifier : j)
  {
    CHECK_STRING(j_qualifier);
    CHECK(table.contains(j_qualifierVal), "Unknown token {}", j_qualifierVal);
    out.emplace(table.at(j_qualifierVal));
  }

  return true;
}

PARSER_FUNC_TYPE(ParseStorageBufferVariable, ShaderData::StorageBufferVariable)
{
  CHECK(ParseVariable(type, j, out), "Error while parsing variable");

  IF_HAS_ATTR(j, Qualifiers)
    CHECK(ParseMemoryQualifiers(type, Qualifiers, out.memoryQualifiers), "Error while parsing memory qualifiers");

  return true;
}

PARSER_FUNC(ParseUniformBuffer)
{
  CHECK_OBJECT(j);

  CHECK_ATTR_STRING(j, Name);
  CHECK_ATTR_STRING(j, Layout);
  CHECK_ATTR_ARRAY(j, Variables);

  ShaderData::UniformBuffer ub;
  ub.name = NameVal;
  ub.layout = LayoutVal;
  
  IF_HAS_ATTR_STRING(j, EnableIf)
    ub.enableIf = EnableIfVal;
  
  for (const auto& j_variable : VariablesVal)
  {
    auto& v = ub.variables.emplace_back();
    CHECK(ParseVariable(type, j_variable, v), "Error while parsing variable");
  }

  return true;
}

PARSER_FUNC(ParseUniformBuffers)
{
  CHECK_ARRAY(j);

  for (const auto& j_uniformBuffer : j)
  {
    CHECK(ParseUniformBuffer(type, j_uniformBuffer, out), "Error while parsing uniform buffer");
  }

  return true;
}

PARSER_FUNC(ParseStorageBuffer)
{
  CHECK_OBJECT(j);

  ShaderData::StorageBuffer sb;

  CHECK_ATTR_STRING(j, Name);
  CHECK_ATTR_STRING(j, Layout);
  CHECK_ATTR_ARRAY(j, Variables);

  sb.name = NameVal;
  sb.layout = LayoutVal;

  for (const auto& j_variable : VariablesVal)
  {
    auto& v = sb.variables.emplace_back();
    CHECK(ParseStorageBufferVariable(type, j_variable, v), "Error while parsing storage buffer variable");
  }
  
  IF_HAS_ATTR(j, Qualifiers)
    CHECK(ParseMemoryQualifiers(type, Qualifiers, sb.memoryQualifiers), "Error while parsing memory qualifiers");
  
  IF_HAS_ATTR_STRING(j, EnableIf)
    sb.enableIf = EnableIfVal;

  out.addStorageBuffer(type, sb);

  return true;
}

PARSER_FUNC(ParseStorageBuffers)
{
  CHECK_ARRAY(j);

  for (const auto& j_storageBuffer : j)
  {
    CHECK(ParseStorageBuffer(type, j_storageBuffer, out), "Error while parsing storage buffer");
  }

  return true;
}

PARSER_FUNC(ParseVertexAttribute)
{
  CHECK_OBJECT(j);

  ShaderData::VertexAttribute va;

  CHECK(ParseVariable(type, j, va), "Error while parsing variable");

  IF_HAS_ATTR_STRING(j, Layout)
    va.layout = LayoutVal;

  out.addVertexAttribute(va);

  return true;
}

PARSER_FUNC(ParseVertexAttributes)
{
  CHECK_ARRAY(j);

  for (const auto& j_va : j)
  {
    CHECK(ParseVertexAttribute(type, j_va, out), "Error while parsing vertex attribute");
  }

  return true;
}

PARSER_FUNC(ParseShader)
{
  CHECK_OBJECT(j);

  IF_HAS_ATTR_STRING(j, Version)
    out.setVersion(type, VersionVal);

  IF_HAS_ATTR(j, Defines)
    CHECK(ParseDefines(type, Defines, out), "Error while parsing defines");

  IF_HAS_ATTR(j, VertexAttributes)
    CHECK(ParseVertexAttributes(type, VertexAttributes, out), "Error while parsing vertex attributes");

  IF_HAS_ATTR(j, Uniforms)
    CHECK(ParseUniforms(type, Uniforms, out), "Error while parsing uniforms");

  IF_HAS_ATTR(j, Extensions)
    CHECK(ParseExtensions(type, Extensions, out), "Error while parsing extensions");

  IF_HAS_ATTR(j, Sources)
    CHECK(ParseSources(type, Sources, out), "Error while parsing sources");

  IF_HAS_ATTR(j, EarlySources)
    CHECK(ParseEarlySources(type, EarlySources, out), "Error while parsing early sources");
  
  IF_HAS_ATTR(j, UniformBuffers)
    CHECK(ParseUniformBuffers(type, UniformBuffers, out), "Error while parsing uniform buffers");

  IF_HAS_ATTR(j, StorageBuffers)
    CHECK(ParseStorageBuffers(type, StorageBuffers, out), "Error while parsing storage buffers");

  return true;
}

PARSER_FUNC(ParseVarying)
{
  CHECK_OBJECT(j);

  ShaderData::Varying v;

  CHECK(ParseVariable(type, j, v), "Error while parsing variable");
 
  static std::unordered_map<std::string, ShaderData::Varying::EQualifier> table =
  {
    { "flat", ShaderData::Varying::EQualifier::eSmooth },
    { "smooth", ShaderData::Varying::EQualifier::eFlat },
    { "noperspective", ShaderData::Varying::EQualifier::eNoPerspective },
  };

  IF_HAS_ATTR_STRING(j, Qualifier)
  {
    CHECK(table.contains(QualifierVal), "Unknown token {}", QualifierVal);
    v.qualifier = table.at(QualifierVal);
  }

  out.addVarying(v);

  return true;
}

PARSER_FUNC(ParseVaryings)
{
  CHECK_ARRAY(j);

  for (const auto& j_varying : j)
  {
    CHECK(ParseVarying(type, j_varying, out), "Error while parsing varying");
  }

  return true;
}

bool ShaderParsing::Parse(const json& j, ShaderData& out)
{
  ShaderData data;

  CHECK_OBJECT(j);

  std::vector<std::pair<std::string, ShaderData::EShaderType>> textToType = {
    {"Global", ShaderData::EShaderType::eAll},
    {"Compute", ShaderData::EShaderType::eCompute},
    {"Vertex", ShaderData::EShaderType::eVertex},
    {"Geometry", ShaderData::EShaderType::eGeometry},
    {"Fragment", ShaderData::EShaderType::eFragment},
  };

  for (const auto& [str, type] : textToType)
  {
    if (j.contains(str))
    {
      CHECK(ParseShader(type, j.at(str), data), "Error while parsing shader");
      data.setShaderEnabled(type, true);
    }
  }

  IF_HAS_ATTR(j, Varyings)
  {
    CHECK(ParseVaryings(ShaderData::EShaderType::eNone, Varyings, data), "Error while parsing varyings");
  }

  out = std::move(data);

  return true;
}
