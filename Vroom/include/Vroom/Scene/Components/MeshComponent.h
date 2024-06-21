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
    MeshComponent() = default;

    /**
     * @brief Constructor.
     * 
     * @param meshInstance The mesh instance.
     */
    MeshComponent(const MeshInstance& meshInstance);

    const MeshData& getMeshData() const;

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

private:
    MeshInstance m_MeshInstance;
};

} // namespace vrm