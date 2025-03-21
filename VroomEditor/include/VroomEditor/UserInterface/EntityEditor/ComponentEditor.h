#pragma once

#include <Vroom/Scene/Entity.h>

namespace vrm
{

template <typename C>
class ComponentEditor
{
public:

  void editEntityComponent(Entity e) const
  {
    if (e.hasComponent<C>())
      onVisualizeComponent(e.getComponent<C>());
  }

protected:

  virtual void onVisualizeComponent(C& component) = 0;

private:

};

} // namespace vrm
