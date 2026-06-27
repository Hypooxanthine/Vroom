#pragma once

#include <vector>
#include "Physics/PhysicsLayer.h"

namespace vrm
{

/**
 * @brief You need to first add all the layers, then generate collision maps, and then set collisions between layers.
 * UB if this sequence is not observed.
 *
 */
class PhysicsLayerList
{
public:

  PhysicsLayerList();

  const std::vector<PhysicsLayer>& getLayers() const;
  size_t                           getLayerCount() const;

  /**
   * @brief Add a layer to the list
   *
   * @param layer
   * @return size_t The identifier of the layer, useful for setting layers collisions
   */
  size_t addLayer(PhysicsLayer&& layer);

  /**
   * @brief Needs to be called before the first "setColliding" call
   *
   */
  void generateCollisionMaps();

  void setColliding(size_t layerId1, size_t layerId2, bool colliding = true);

private:

  std::vector<PhysicsLayer> m_layers;
};

} // namespace vrm
