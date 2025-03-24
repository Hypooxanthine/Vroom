#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <array>
#include <filesystem>

namespace vrm
{

  class ShaderData
  {
  public:
    enum class EShaderType : uint8_t
    {
      eNone = 0, eCompute, eVertex, eGeometry, eFragment, eCount, eAll
    };

    struct Extension
    {
      enum class EBehaviour : uint8_t
      {
        eEnable, eRequire, eWarn, eDisable
      };
      std::string name;
      EBehaviour behaviour;
      std::string enableIf;
    };

    struct SourceElement
    {
      std::string content;
      std::string filePath;
      std::string enableIf;
    };

    struct Define
    {
      std::string name;
      std::string value;
    };

    enum class EMemoryQualifier : uint8_t
    {
      eCoherent, eVolatile, eRestrict, eReadOnly, eWriteOnly
    };

    struct Varying
    {
      enum class EQualifier : uint8_t
      {
        eFlat, eSmooth, eNoPerspective
      };

      std::string name;
      std::string type;
      EQualifier qualifier = EQualifier::eSmooth;
      std::string enableIf;
    };

    struct Variable
    {
      std::string name;
      std::string type;
      std::string enableIf;
    };

    using Uniform = Variable;

    struct UniformBuffer
    {
      std::string layout;
      std::string name;
      std::vector<Variable> variables;
      std::string enableIf;
    };

    struct StorageBufferVariable : Variable
    {
      std::unordered_set<EMemoryQualifier> memoryQualifiers;
    };

    struct StorageBuffer
    {
      std::string layout;
      std::unordered_set<EMemoryQualifier> memoryQualifiers;
      std::string name;
      std::vector<StorageBufferVariable> variables;
      std::string enableIf;
    };

    struct ShaderProperties
    {
      bool enable = false;
      std::string version;
      std::vector<Extension> extensions;
      std::vector<SourceElement> earlySources;
      std::vector<SourceElement> sources;
      std::vector<Define> defines;
      std::vector<Uniform> uniforms;
      std::vector<UniformBuffer> uniformBuffers;
      std::vector<StorageBuffer> storageBuffers;
    };

    struct CombinedShader
    {
      EShaderType type;
      std::string sourceCode;
    };

    struct FullShader
    {
      FullShader() :
          compute{ EShaderType::eCompute, "" }
        , vertex{ EShaderType::eVertex, "" }
        , geometry{ EShaderType::eGeometry, "" }
        , fragment{ EShaderType::eFragment, "" }
      {}

      FullShader(const FullShader& other) : shaders(other.shaders) {}
      FullShader(FullShader&& other) noexcept : shaders(std::move(other.shaders)) {}

      ~FullShader() noexcept {}

      union
      {
        std::array<CombinedShader, size_t(EShaderType::eCount)> shaders;
        struct
        {
          CombinedShader compute;
          CombinedShader vertex;
          CombinedShader geometry;
          CombinedShader fragment;
        };
      };

      std::string& operator[](const EShaderType& type)
      {
        switch (type)
        {
        case EShaderType::eCompute: return compute.sourceCode;
        case EShaderType::eVertex: return vertex.sourceCode;
        case EShaderType::eGeometry: return geometry.sourceCode;
        case EShaderType::eFragment: return fragment.sourceCode;
        default: return compute.sourceCode;
        }
      }
    };

  public:

    void setShaderEnabled(const EShaderType& shader, const bool enable);
    void setVersion(const EShaderType& shader, const std::string& version);
    void addExtension(const EShaderType& shader, const Extension& ext);
    void addDefine(const EShaderType& shader, const Define& define);
    void addVarying(const EShaderType& shader, const Varying& varying);
    void addUniform(const EShaderType& shader, const Uniform& uniform);
    void addUniformBuffer(const EShaderType& shader, const UniformBuffer& uniformBuffer);
    void addStorageBuffer(const EShaderType& shader, const StorageBuffer& storageBuffer);
    void addEarlySource(const EShaderType& shader, const SourceElement& source);
    void addSource(const EShaderType& shader, const SourceElement& source);

    bool validate() const;
    FullShader combine() const;

  private:
    ShaderProperties& getProps(const EShaderType& shader);
    const ShaderProperties& getProps(const EShaderType& shader) const;

    std::string combine(const ShaderProperties& global, const ShaderProperties& shader, const EShaderType& shaderType, const std::vector<Varying>& varyings = {}) const;

    std::string combineVersion(const std::string& global, const std::string& shader) const;
    std::string combineExtensions(const std::vector<Extension>& global, const std::vector<Extension>& shader) const;
    std::string combineDefines(const std::vector<Define>& global, const std::vector<Define>& shader) const;
    std::string combineVaryings(const std::string& prefix, const std::vector<Varying>& varyings) const;
    std::string combineUniforms(const std::vector<Uniform>& global, const std::vector<Uniform>& shader) const;
    std::string combineUniformBuffers(const std::vector<UniformBuffer>& global, const std::vector<UniformBuffer>& shader) const;
    std::string combineStorageBuffers(const std::vector<StorageBuffer>& global, const std::vector<StorageBuffer>& shader) const;
    std::string combineSources(const std::vector<SourceElement>& global, const std::vector<SourceElement>& shader) const;

    std::string applyEnableIf(const std::string& source, const std::string& enableIf) const;

    std::string toString(const EMemoryQualifier& qualifier) const;
    std::string toString(const Varying::EQualifier& qualifier) const;
    std::string toString(const Extension::EBehaviour& behaviour) const;

  private:
    ShaderProperties m_globalProperties;
    std::array<ShaderProperties, size_t(EShaderType::eCount)> m_shaderProps;
    std::vector<Varying> m_varyings;
  };

} // namespace vrm
