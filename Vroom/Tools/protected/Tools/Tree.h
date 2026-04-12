#pragma once

#include <memory>

namespace vrm
{

template <typename T>
class Tree
{
public:

  enum class EIterOrder : uint8_t
  {
    eDown,
    eUp
  };

public:
  inline Tree() = default;

  template <typename... Args>
  inline Tree(Args&&... args);

  inline Tree(const T& data);
  inline Tree(T&& data);
  inline Tree(const Tree& other);
  inline Tree(Tree&& other);
  inline Tree& operator=(const Tree& other);
  inline Tree& operator=(Tree&& other);

  inline T& data();
  inline const T& data() const;

  template <typename Cond>
  inline Tree* find(const Cond& cond);

  template <typename Func, EIterOrder Order>
  inline void forEach(const Func& f);

  template <typename Func>
  inline void forEachDownward(const Func& f);

  template <typename Func>
  inline void forEachUpward(const Func& f);

  template <typename... Args>
  inline Tree& emplaceNeighbour(Args&&... args);

  template <typename... Args>
  inline Tree& emplaceChild(Args&&... args);

  inline bool isLeaf() const;

protected:

private:

  // Iteration data

  Tree* m_parent = nullptr;
  std::unique_ptr<Tree> m_child = nullptr;
  std::unique_ptr<Tree> m_neighbour = nullptr;

  // Node data

  T m_data;
};

template <typename T>
template <typename... Args>
inline Tree<T>::Tree(Args&&... args)
  : m_data(std::forward<Args>(args)...)
{
}

template <typename T>
inline Tree<T>::Tree(const T& data)
  : m_data(data)
{
}

template <typename T>
inline Tree<T>::Tree(T&& data)
  : m_data(std::move(data))
{
}

template <typename T>
inline Tree<T>::Tree(const Tree& other)
{
  *this = other;
}

template <typename T>
inline Tree<T>::Tree(Tree&& other)
{
  *this = std::move(other);
}

template <typename T>
inline Tree<T>& Tree<T>::operator=(const Tree& other)
{
  if (*this != &other)
  {
    m_data = other.m_data;

    if (other.m_child != nullptr)
      m_child = std::make_unique<T>(*other.m_child.get());
    
    if (other.m_neighbour != nullptr)
      m_neighbour = std::make_unique<T>(*other.m_neighbour.get());
  }

  return *this;
}

template <typename T>
inline Tree<T>& Tree<T>::operator=(Tree&& other)
{
  if (this != &other)
  {
    // Remove other from its tree


    m_data = std::move(other.m_data);
    m_parent = std::move(other.m_parent);
    m_child = std::move(other.m_child);
    m_neighbour = std::move(other.m_neighbour);
  
    other.m_parent = nullptr;
    other.m_child = nullptr;
    other.m_neighbour = nullptr;
  }

  return *this;
}

template <typename T>
inline T& Tree<T>::data()
{
  return m_data;
}

template <typename T>
inline const T& Tree<T>::data() const
{
  return m_data;
}

template <typename T>
template <typename Cond>
inline Tree<T>* Tree<T>::find(const Cond& cond)
{
  if (cond(m_data))
    return this;

  if (m_child != nullptr)
  {
    auto* childRet = m_child->find(cond);
    if (childRet)
      return childRet;
  }

  if (m_neighbour != nullptr)
  {
    auto* neighbourRet = m_neighbour->find(cond);
    if (neighbourRet)
      return neighbourRet;
  }
  
  return nullptr;
}

template <typename T>
template <typename Func, typename Tree<T>::EIterOrder Order>
inline void Tree<T>::forEach(const Func& f)
{
  if (Order == EIterOrder::eDown)
    f(m_data);

  if (m_child != nullptr)
    m_child->forEachDownwards(f);
  
  if (m_neighbour != nullptr)
    m_child->forEachDownwards(f);

  if (Order == EIterOrder::eUp)
    f(m_data);
}

template <typename T>
template <typename Func>
inline void Tree<T>::forEachDownward(const Func& f)
{
  forEach<EIterOrder::eDown>(f);
}

template <typename T>
template <typename Func>
inline void Tree<T>::forEachUpward(const Func& f)
{
  forEach<EIterOrder::eUp>(f);
}

template <typename T>
template <typename... Args>
inline Tree<T>& Tree<T>::emplaceNeighbour(Args&&... args)
{
  auto node = std::make_unique<Tree>(std::forward<Args>(args)...);

  if (m_neighbour)
    node.get()->m_neighbour = std::move(m_neighbour);

  m_neighbour = std::move(node);

  return *m_neighbour.get();
}

template <typename T>
template <typename... Args>
inline Tree<T>& Tree<T>::emplaceChild(Args&&... args)
{
  if (!m_child)
  {
    m_child = std::make_unique<Tree>(std::forward<Args>(args)...);
    return m_child;
  }

  auto* child = m_child.get();
  while (child->m_neighour != nullptr)
    child = child->m_neighbour;

  return child->emplaceNeighbour(std::forward<Args>(args)...);
}

template <typename T>
inline bool Tree<T>::isLeaf() const
{
  return m_child == nullptr;
}

} // namespace vrm
