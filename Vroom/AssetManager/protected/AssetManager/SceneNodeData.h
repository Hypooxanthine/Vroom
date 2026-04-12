#pragma once

#include <cstdint>
#include <string>

#include "AssetManager/ComponentDataMap.h"

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
    
    SceneNodeData& operator=(const SceneNodeData& other) = default;
    SceneNodeData(const SceneNodeData&) = default;

    SceneNodeData& operator=(SceneNodeData&& other) = default;
    SceneNodeData(SceneNodeData&& other) = default;

    template <typename T, typename... Args>
    T& emplaceComponent(Args&&... args)
    {
      auto comp = std::make_unique<T>(std::forward<Args>(args)...);
      T& ref = *comp;
      components[std::type_index(typeid(T))] = std::move(comp);
      return ref;
    }

    template <typename T>
    const T* tryGetComponent() const
    {
      const T* component = nullptr;

      auto typeIndex = std::type_index(typeid(T));
      if (auto it = components.find(typeIndex); it != components.end())
      {
        component = static_cast<const T*>(it->second.get());
      }

      return component;
    }

    std::string name;
    EType type;
    std::string parent;
    ComponentDataMap components;
  };

}
