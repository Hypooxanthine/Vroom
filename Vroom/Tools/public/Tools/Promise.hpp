#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace vrm
{

template <typename T>
class Promise;

template <typename T>
class Future;

namespace detail
{

/**
 * @brief Unwraps a Promise<U> into U. Leaves any other type untouched.
 *        Used by Promise::then to flatten promise-returning callbacks.
 */
template <typename U>
struct PromiseUnwrap
{
  using Type = U;
  static constexpr bool isPromise = false;
};

template <typename U>
struct PromiseUnwrap<Promise<U>>
{
  using Type = U;
  static constexpr bool isPromise = true;
};

/**
 * @brief State shared between a Future (producer) and its Promise(s) (consumers).
 *
 * This is purposely single threaded: resolving the state immediately invokes
 * every registered continuation, and a continuation registered after the state
 * has been resolved is invoked on the spot.
 */
template <typename T>
struct PromiseState
{
  std::optional<T> value;
  std::vector<std::function<void(const T&)>> continuations;

  bool resolved() const { return value.has_value(); }

  template <typename U>
  void resolve(U&& v)
  {
    assert(!resolved() && "Promise resolved more than once");
    value.emplace(std::forward<U>(v));
    fire();
  }

  void subscribe(std::function<void(const T&)> continuation)
  {
    if (resolved())
      continuation(*value);
    else
      continuations.emplace_back(std::move(continuation));
  }

private:

  void fire()
  {
    auto pending = std::move(continuations);
    continuations.clear();
    for (auto& continuation : pending)
      continuation(*value);
  }
};

template <>
struct PromiseState<void>
{
  bool valueSet = false;
  std::vector<std::function<void()>> continuations;

  bool resolved() const { return valueSet; }

  void resolve()
  {
    assert(!resolved() && "Promise resolved more than once");
    valueSet = true;
    fire();
  }

  void subscribe(std::function<void()> continuation)
  {
    if (resolved())
      continuation();
    else
      continuations.emplace_back(std::move(continuation));
  }

private:

  void fire()
  {
    auto pending = std::move(continuations);
    continuations.clear();
    for (auto& continuation : pending)
      continuation();
  }
};

} // namespace detail

/**
 * @brief Consumer side of an asynchronous result.
 *
 * A Promise is obtained from a Future via Future::createPromise(). It is a cheap
 * handle (shared pointer) over the shared state, so it can be freely copied,
 * stored and have then() called on it several times.
 *
 * @tparam T The type the asynchronous operation resolves to (may be void).
 */
template <typename T>
class Promise
{
public:

  using ValueType = T;

  Promise()                          = default;
  Promise(const Promise&)            = default;
  Promise(Promise&&)                 = default;
  Promise& operator=(const Promise&) = default;
  Promise& operator=(Promise&&)      = default;

  /**
   * @brief Registers a continuation that runs once this promise resolves.
   *
   * The callback receives the resolved value (const T&). Its return type drives
   * the returned promise:
   *  - returns U          -> then() returns Promise<U>
   *  - returns Promise<U> -> then() returns Promise<U> (the chain is flattened)
   *  - returns void       -> then() returns Promise<void>
   *
   * If this promise is already resolved, the callback runs immediately.
   *
   * @return A promise resolved with the result of the callback.
   */
  template <typename F>
  auto then(F&& onResolved) const;

  /// @brief Whether the underlying asynchronous result is available.
  bool isResolved() const;

  /// @brief Whether this promise refers to a valid shared state.
  bool isValid() const { return static_cast<bool>(m_state); }

  /// @brief Direct access to the resolved value, empty until resolved.
  const std::optional<T>& peek() const;

private:

  template <typename U>
  friend class Promise;
  template <typename U>
  friend class Future;

  using StatePtr = std::shared_ptr<detail::PromiseState<T>>;

  explicit Promise(StatePtr state) : m_state(std::move(state)) {}

  StatePtr m_state;
};

template <>
class Promise<void>
{
public:

  using ValueType = void;

  Promise()                          = default;
  Promise(const Promise&)            = default;
  Promise(Promise&&)                 = default;
  Promise& operator=(const Promise&) = default;
  Promise& operator=(Promise&&)      = default;

  template <typename F>
  auto then(F&& onResolved) const;

  bool isResolved() const;

  bool isValid() const { return static_cast<bool>(m_state); }

private:

  template <typename U>
  friend class Promise;
  template <typename U>
  friend class Future;

  using StatePtr = std::shared_ptr<detail::PromiseState<void>>;

  explicit Promise(StatePtr state) : m_state(std::move(state)) {}

  StatePtr m_state;
};

/**
 * @brief Producer side of an asynchronous result.
 *
 * The code performing the asynchronous work (e.g. an http request) keeps the
 * Future alive, hands out Promise handles via createPromise(), and calls
 * setValue() once the result is ready. A Future is move only: it represents the
 * unique right to resolve the shared state.
 *
 * @tparam T The type the asynchronous operation resolves to (may be void).
 */
template <typename T>
class Future
{
public:

  Future();

  Future(const Future&)            = delete;
  Future& operator=(const Future&) = delete;
  Future(Future&&)                 = default;
  Future& operator=(Future&&)      = default;

  /// @brief Creates a consumer handle bound to this future's shared state.
  Promise<T> createPromise() const;

  /// @brief Resolves the shared state and runs every pending continuation.
  void setValue(const T& value);
  void setValue(T&& value);

  bool isResolved() const;

private:

  std::shared_ptr<detail::PromiseState<T>> m_state;
};

template <>
class Future<void>
{
public:

  Future();

  Future(const Future&)            = delete;
  Future& operator=(const Future&) = delete;
  Future(Future&&)                 = default;
  Future& operator=(Future&&)      = default;

  Promise<void> createPromise() const;

  void setValue();

  bool isResolved() const;

private:

  std::shared_ptr<detail::PromiseState<void>> m_state;
};

} // namespace vrm

#include "Tools/Promise.inl"
