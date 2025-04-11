#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <typeindex>

#include <glm/glm.hpp>

namespace vrm
{

  class Entity;

  class SceneData
  {
  public:

    struct Component
    {
      virtual bool addToEntity(Entity& entity) = 0;
    };

    struct TransformComponent : Component
    {
      glm::vec3 position;
      glm::vec3 rotation;
      glm::vec3 scale;

      bool addToEntity(Entity& entity) override;
    };

    struct MeshComponent : Component
    {
      std::string resourceName;

      bool addToEntity(Entity& entity) override;
    };

    struct DirectionalLightComponent : Component
    {
      glm::vec3 color;
      float intensity;

      bool addToEntity(Entity& entity) override;
    };

    struct PointLightComponent : Component
    {
      glm::vec3 color;
      float intensity;
      float radius;

      bool addToEntity(Entity& entity) override;
    };

    struct ScriptComponent : Component
    {
      std::string resourceName;

      bool addToEntity(Entity& entity) override;
    };

    struct SceneNode
    {
      enum class EType : uint8_t
      {
        eRoot, eEntity
      };

      SceneNode() = default;

      ~SceneNode() = default;

      SceneNode(const std::string& name, EType type, const std::string& parent)
        : name(name)
        , type(type)
        , parent(parent)
      {}

      SceneNode(SceneNode&& other)
        : name(std::move(other.name))
        , type(other.type)
        , parent(std::move(other.parent))
        , components(std::move(other.components))
      {}

      SceneNode& operator=(SceneNode&& other)
      {
        name = std::move(other.name);
        type = other.type;
        parent = std::move(other.parent);
        components = std::move(other.components);
        return *this;
      }

      std::string name;
      EType type;
      std::string parent;
      std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
    };

  public:
    inline SceneData() = default;

    inline ~SceneData() = default;

    inline SceneData& operator=(SceneData&& other)
    {
      m_nodes = std::move(other.m_nodes);
      return *this;
    }

    inline const auto& getNodes() const { return m_nodes; }

    inline void reset()
    {
      m_nodes.clear();
    }

    inline size_t addNode(SceneNode&& node)
    {
      m_nodes.emplace_back(std::move(node));
      return m_nodes.size() - 1;
    }

    inline size_t addNode(const std::string& name, SceneNode::EType type, const std::string& parent)
    {
      return addNode(SceneNode{ name, type, parent });
    }

    inline void addComponent(size_t nodeIndex, std::unique_ptr<Component>&& component)
    {
      m_nodes[nodeIndex].components[std::type_index(typeid(*component))] = std::move(component);
    }

  protected:

  private:
    std::vector<SceneNode> m_nodes;

  };

} // namespace vrm
