#pragma once

#include <cstdint>
#include <string>

#include "Vroom/Asset/AssetData/ComponentDataMap.h"

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

    std::string name;
    EType type;
    std::string parent;
    ComponentDataMap components;
  };

}
