#include "Vroom/Asset/Parsing/SceneNodeParsing.h"

#include "Vroom/Asset/Parsing/ComponentParsing.h"

using namespace vrm;

namespace nlohmann
{

  void to_json(json& j, const SceneNodeData& node)
  {
    j = json::object();
    j["name"] = node.name;
    j["type"] = node.type;

    auto& j_parents = (j["parent"] = nullptr);

    if (!node.parent.empty())
    {
      j_parents = node.parent;
    }

    if (node.components.empty())
    {
      return;
    }

    auto& j_components = (j["components"] = json::array());

    for (const auto& [_, component] : node.components)
    {
      json& j = j_components.emplace_back();
      j = *component.get();
    }
  }

  void from_json(const json& j, SceneNodeData& node)
  {
    VRM_CHECK_THROW_MSG(j.is_object(), "Expected an object for SceneNodeData, got {}", j.dump(2));
    VRM_CHECK_THROW_MSG(j.contains("name"), "SceneNodeData: Missing 'name' field");
    VRM_CHECK_THROW_MSG(j.contains("type"), "SceneNodeData: Missing 'type' field");
    VRM_CHECK_THROW_MSG(j.contains("parent"), "SceneNodeData: Missing 'parent' field");

    node.name = j["name"];
    node.type = j["type"].get<SceneNodeData::EType>();
    
    if (node.type == SceneNodeData::EType::eRoot)
    {
      VRM_CHECK_THROW_MSG(node.name == "Root", "Root node must be named 'Root', got '{}'", node.name);
      VRM_CHECK_THROW_MSG(j["parent"].is_null(), "Root node cannot specify a parent node");
      return;
    }
    else
    {
      node.parent = j["parent"];
      VRM_CHECK_THROW_MSG(!node.name.empty(), "SceneNodeData: Node name cannot be empty");
    }

    if (j.contains("components"))
    {
      const auto& components = j["components"];
      for (const auto& component : components)
      {
        VRM_CHECK_THROW_MSG(component.is_object(), "Unexpected component {}", component.dump(2));
        std::unique_ptr<ComponentData> compData;
        compData = component;
        VRM_CHECK_THROW_MSG(compData != nullptr, "Failed to deserialize component data");

        auto* comp = compData.get();
        auto typeIndex = std::type_index(typeid(*comp));
        if (node.components.find(typeIndex) != node.components.end())
        {
          VRM_LOG_ERROR("Component of type {} already exists in SceneNodeData '{}'", typeIndex.name(), node.name);
          continue; // Skip if component already exists
        }

        node.components[typeIndex] = std::move(compData);
      }
    }
  }

} // namespace nlohmann