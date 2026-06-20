#include <gtest/gtest.h>
#include <Tools/Promise.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

using namespace vrm;

TEST(test_Promise, ResolveThenContinuation)
{
  Future<int> fut;
  Promise<int> p = fut.createPromise();

  int seen = 0;
  p.then([&](const int& v) { seen = v; });

  EXPECT_FALSE(p.isResolved());
  fut.setValue(42);
  EXPECT_TRUE(p.isResolved());
  EXPECT_EQ(seen, 42);
  EXPECT_EQ(p.peek().value(), 42);
}

TEST(test_Promise, ThenAfterResolveRunsImmediately)
{
  Future<int> fut;
  fut.setValue(7);
  Promise<int> p = fut.createPromise();

  int seen = 0;
  p.then([&](const int& v) { seen = v; });
  EXPECT_EQ(seen, 7);
}

TEST(test_Promise, ValueChaining)
{
  Future<int> fut;
  auto p = fut.createPromise();

  std::string result;
  p.then([](const int& v) { return v * 2; })
   .then([](const int& v) { return std::string("v=") + std::to_string(v); })
   .then([&](const std::string& s) { result = s; });

  fut.setValue(21);
  EXPECT_EQ(result, "v=42");
}

// Minimal async producer that resolves its promises only when flushed,
// emulating an http request whose response arrives later.
namespace
{

struct FakeHttp
{
  std::vector<std::function<void()>> pending;

  Promise<std::string> get(std::string body)
  {
    auto fut = std::make_shared<Future<std::string>>();
    pending.push_back([fut, body]() { fut->setValue(body); });
    return fut->createPromise();
  }

  void flush()
  {
    auto batch = std::move(pending);
    pending.clear();
    for (auto& fn : batch)
      fn();
  }
};

} // namespace

TEST(test_Promise, FlattensPromiseReturningCallback)
{
  FakeHttp http;

  std::string out;
  http.get("first")
      .then([&](const std::string& s) { return http.get(s + "->second"); })
      .then([&](const std::string& s) { out = s; });

  EXPECT_TRUE(out.empty());
  http.flush(); // resolves "first"
  EXPECT_TRUE(out.empty()); // chained request not resolved yet
  http.flush(); // resolves the chained request
  EXPECT_EQ(out, "first->second");
}

TEST(test_Promise, VoidReturningContinuation)
{
  Future<int> fut;
  auto p = fut.createPromise();

  int calls = 0;
  p.then([&](const int&) { calls++; }) // returns void -> Promise<void>
   .then([&]() { calls++; });          // Promise<void>::then

  fut.setValue(1);
  EXPECT_EQ(calls, 2);
}

TEST(test_Promise, FutureVoid)
{
  Future<void> fut;
  auto p = fut.createPromise();

  bool ran = false;
  int chained = 0;
  p.then([&]() { ran = true; return 5; })
   .then([&](const int& v) { chained = v; });

  EXPECT_FALSE(ran);
  fut.setValue();
  EXPECT_TRUE(ran);
  EXPECT_EQ(chained, 5);
}

TEST(test_Promise, FlattensVoidPromise)
{
  Future<int> fut;
  auto p = fut.createPromise();

  Future<void> inner;
  bool done = false;

  p.then([&](const int&) { return inner.createPromise(); }) // returns Promise<void>
   .then([&]() { done = true; });

  fut.setValue(1);
  EXPECT_FALSE(done); // waiting on the inner promise
  inner.setValue();
  EXPECT_TRUE(done);
}

TEST(test_Promise, MultipleContinuationsOnSamePromise)
{
  Future<int> fut;
  auto p = fut.createPromise();

  int a = 0, b = 0;
  p.then([&](const int& v) { a = v; });
  p.then([&](const int& v) { b = v + 1; });

  fut.setValue(10);
  EXPECT_EQ(a, 10);
  EXPECT_EQ(b, 11);
}
