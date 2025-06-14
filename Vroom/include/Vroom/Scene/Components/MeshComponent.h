#pragma once

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Render/MeshMaterials.h"

namespace vrm
{

  class MeshData;

  /**
   * @brief Mesh component.
   *
   * A mesh component is a component that stores the mesh of an entity.
   */
  class VRM_API MeshComponent
  {
  public:
    MeshComponent();

    /**
     * @brief Constructor.
     *
     * @param meshInstance The mesh instance.
     */
    MeshComponent(const MeshAsset::Handle &meshInstance);

    /**
     * @brief Get the mesh instance.
     *
     * @return The mesh instance.
     */
    const MeshAsset::Handle &getMesh() const;

    /**
     * @brief Set the mesh instance.
     *
     * @param meshInstance The mesh instance.
     */
    void setMesh(const MeshAsset::Handle &meshInstance);

    /**
     * @brief Get materials of this mesh component. Could be default ones, or overriden.
     * 
     * @return const MeshMaterials& The materials.
     */
    inline const MeshMaterials& getMaterials() const { return m_Materials; }

    void setMaterial(size_t slot, MaterialAsset::Handle mat);

    /**
     * @brief Set the visibility of the mesh.
     *
     * @param visible True if the mesh is visible, false otherwise.
     */
    inline void setVisible(bool visible) { m_IsVisible = visible; }

    /**
     * @brief Check if the mesh is visible.
     *
     * @return True if the mesh is visible, false otherwise.
     */
    inline bool isVisible() const { return m_IsVisible; }

    inline void setCastsShadow(bool value) { m_castsShadow = value; }

    inline bool doesCastShadow() const { return m_castsShadow; }

  private:
    MeshAsset::Handle m_MeshInstance;
    MeshMaterials m_Materials;
    bool m_IsVisible = true;
    bool m_castsShadow = true;
  };

} // namespace vrm