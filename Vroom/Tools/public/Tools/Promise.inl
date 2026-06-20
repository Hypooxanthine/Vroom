#pragma once

#include "Tools/Promise.hpp"

namespace vrm
{

// ---------------------------------------------------------------------------
// Promise<T>
// ---------------------------------------------------------------------------

template <typename T>
template <typename F>
auto Promise<T>::then(F&& onResolved) const
{
  using Raw    = std::invoke_result_t<F, const T&>;
  using Result = typename detail::PromiseUnwrap<Raw>::Type;

  assert(isValid() && "then() called on an empty promise");

  auto next = std::make_shared<detail::PromiseState<Result>>();

  m_state->subscribe(
    [next, callback = std::forward<F>(onResolved)](const T& value) mutable
    {
      if constexpr (std::is_void_v<Raw>)
      {
        callback(value);
        next->resolve();
      }
      else if constexpr (detail::PromiseUnwrap<Raw>::isPromise)
      {
        // Callback produced another promise: flatten it into our result.
        callback(value).then(
          [next](auto&&... inner)
          {
            next->resolve(std::forward<decltype(inner)>(inner)...);
          });
      }
      else
      {
        next->resolve(callback(value));
      }
    });

  return Promise<Result>(next);
}

template <typename T>
bool Promise<T>::isResolved() const
{
  return m_state && m_state->resolved();
}

template <typename T>
const std::optional<T>& Promise<T>::peek() const
{
  assert(isValid() && "peek() called on an empty promise");
  return m_state->value;
}

// ---------------------------------------------------------------------------
// Promise<void>
// ---------------------------------------------------------------------------

template <typename F>
auto Promise<void>::then(F&& onResolved) const
{
  using Raw    = std::invoke_result_t<F>;
  using Result = typename detail::PromiseUnwrap<Raw>::Type;

  assert(isValid() && "then() called on an empty promise");

  auto next = std::make_shared<detail::PromiseState<Result>>();

  m_state->subscribe(
    [next, callback = std::forward<F>(onResolved)]() mutable
    {
      if constexpr (std::is_void_v<Raw>)
      {
        callback();
        next->resolve();
      }
      else if constexpr (detail::PromiseUnwrap<Raw>::isPromise)
      {
        callback().then(
          [next](auto&&... inner)
          {
            next->resolve(std::forward<decltype(inner)>(inner)...);
          });
      }
      else
      {
        next->resolve(callback());
      }
    });

  return Promise<Result>(next);
}

inline bool Promise<void>::isResolved() const
{
  return m_state && m_state->resolved();
}

// ---------------------------------------------------------------------------
// Future<T>
// ---------------------------------------------------------------------------

template <typename T>
Future<T>::Future() : m_state(std::make_shared<detail::PromiseState<T>>())
{
}

template <typename T>
Promise<T> Future<T>::createPromise() const
{
  return Promise<T>(m_state);
}

template <typename T>
void Future<T>::setValue(const T& value)
{
  m_state->resolve(value);
}

template <typename T>
void Future<T>::setValue(T&& value)
{
  m_state->resolve(std::move(value));
}

template <typename T>
bool Future<T>::isResolved() const
{
  return m_state->resolved();
}

// ---------------------------------------------------------------------------
// Future<void>
// ---------------------------------------------------------------------------

inline Future<void>::Future() : m_state(std::make_shared<detail::PromiseState<void>>())
{
}

inline Promise<void> Future<void>::createPromise() const
{
  return Promise<void>(m_state);
}

inline void Future<void>::setValue()
{
  m_state->resolve();
}

inline bool Future<void>::isResolved() const
{
  return m_state->resolved();
}

} // namespace vrm
