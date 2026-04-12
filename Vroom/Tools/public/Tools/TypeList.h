#pragma once

#include <type_traits>

namespace vrm
{

  template <typename... Types>
  struct TypeList
  {
    template <typename T>
    static consteval bool HasType()
    {
      return (std::is_same<T, Types>::value || ...);
    }

    template <typename T>
    static consteval auto CreatePushFront()
    {
      return TypeList<T, Types...>();
    }

    template <typename T>
    static consteval auto CreatePushBack()
    {
      return TypeList<Types..., T>();
    }
  };

} // namespace vrm
