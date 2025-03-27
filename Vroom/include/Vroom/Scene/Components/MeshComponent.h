#pragma once

#include "Vroom/Asset/AssetInstance/MeshInstance.h"

namespace vrm
{

class MeshData;

/**
 * @brief Mesh component.
 * 
 * A mesh component is a component that stores the mesh of an entity.
 */
class MeshComponent
{
public:
    MeshComponent();

    /**
     * @brief Constructor.
     * 
     * @param meshInstance The mesh instance.
     */
    MeshComponent(const MeshInstance& meshInstance);

    /**
     * @brief Get the mesh instance.
     * 
     * @return The mesh instance.
     */
    const MeshInstance& getMesh() const;

    /**
     * @brief Set the mesh instance.
     * 
     * @param meshInstance The mesh instance.
     */
    void setMesh(const MeshInstance& meshInstance);

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

private:
    MeshInstance m_MeshInstance;
    bool m_IsVisible = true;
};

} // namespace vrm