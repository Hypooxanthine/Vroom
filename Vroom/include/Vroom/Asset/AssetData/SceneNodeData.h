#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>
#include <memory>
#include <typeindex>

#include "Vroom/Asset/AssetData/ComponentData.h"

namespace vrm
{

  struct SceneNodeData
  {
    enum class EType : uint8_t
    {
      eRoot, eEntity
    };

    SceneNodeData() = default;

    ~SceneNodeData() = default;

    SceneNodeData(const std::string& name, EType type, const std::string& parent)
      : name(name)
      , type(type)
      , parent(parent)
    {
    }

    SceneNodeData(SceneNodeData&& other)
      : name(std::move(other.name))
      , type(other.type)
      , parent(std::move(other.parent))
      , components(std::move(other.components))
    {
    }

    SceneNodeData& operator=(SceneNodeData&& other)
    {
      name = std::move(other.name);
      type = other.type;
      parent = std::move(other.parent);
      components = std::move(other.components);
      return *this;
    }

    template <typename T, typename... Args>
    T& emplaceComponent(Args&&... args)
    {
      auto comp = std::make_unique<T>(std::forward<Args>(args)...);
      T& ref = *comp;
      components[std::type_index(typeid(T))] = std::move(comp);
      return ref;
    }

    std::string name;
    EType type;
    std::string parent;
    std::unordered_map<std::type_index, std::unique_ptr<ComponentData>> components;
  };

}
