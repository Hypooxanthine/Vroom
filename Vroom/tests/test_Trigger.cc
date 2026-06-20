#include <gtest/gtest.h>
#include <functional>

#include <Event/CallbackList.h>
#include <Event/Trigger.h>

using namespace vrm;

// ----- Trigger ----------------------------------------------------------

TEST(test_Trigger, NotTriggeredByDefault)
{
  Trigger t;
  EXPECT_FALSE(t.isTriggered());
}

TEST(test_Trigger, ActivateThenDeactivate)
{
  Trigger t;
  int lastValue = -1;
  int calls     = 0;
  t.bindCallback([&](bool v) { lastValue = v ? 1 : 0; ++calls; }, nullptr);

  t.trigger(true);
  EXPECT_TRUE(t.isTriggered());
  EXPECT_EQ(lastValue, 1);
  EXPECT_EQ(calls, 1);

  t.trigger(false);
  EXPECT_FALSE(t.isTriggered());
  EXPECT_EQ(lastValue, 0);
  EXPECT_EQ(calls, 2);
}

// Two inputs bound to the same trigger: the callback should fire only on the
// 0->1 activation and the 1->0 deactivation, not on every press/release.
TEST(test_Trigger, MultipleActivationsFireCallbackOnce)
{
  Trigger t;
  int activations   = 0;
  int deactivations = 0;
  t.bindCallback([&](bool v) { if (v) ++activations; else ++deactivations; }, nullptr);

  t.trigger(true);  // 0 -> 1 : fires
  t.trigger(true);  // 1 -> 2 : no fire
  EXPECT_TRUE(t.isTriggered());
  EXPECT_EQ(activations, 1);

  t.trigger(false); // 2 -> 1 : no fire
  EXPECT_TRUE(t.isTriggered());
  EXPECT_EQ(deactivations, 0);

  t.trigger(false); // 1 -> 0 : fires
  EXPECT_FALSE(t.isTriggered());
  EXPECT_EQ(deactivations, 1);
}

TEST(test_Trigger, ReleaseWhileInactiveIsNoop)
{
  Trigger t;
  int calls = 0;
  t.bindCallback([&](bool) { ++calls; }, nullptr);

  t.trigger(false);
  EXPECT_FALSE(t.isTriggered());
  EXPECT_EQ(calls, 0);
}

TEST(test_Trigger, RemoveCallbacksFromEmitter)
{
  Trigger t;
  int calls = 0;
  // The emitter pointer is only ever used as a map key, never dereferenced.
  TriggerBinder* emitter = reinterpret_cast<TriggerBinder*>(0x1);
  t.bindCallback([&](bool) { ++calls; }, emitter);

  t.removeCallbacksFromEmitter(emitter);
  t.trigger(true);

  EXPECT_EQ(calls, 0);
}

// ----- CallbackList -----------------------------------------------------

namespace
{
struct DummyBinder
{
};
} // namespace

TEST(test_CallbackList, TriggerAllInvokesEveryCallback)
{
  CallbackList<std::function<void(int)>, DummyBinder> list;
  DummyBinder a, b;
  int sum = 0;
  list.addCallback([&](int x) { sum += x; }, &a);
  list.addCallback([&](int x) { sum += x * 10; }, &b);

  list.triggerAll(5);
  EXPECT_EQ(sum, 5 + 50);
}

TEST(test_CallbackList, UnbindRemovesOnlyThatEmitter)
{
  CallbackList<std::function<void(int)>, DummyBinder> list;
  DummyBinder a, b;
  int aCount = 0, bCount = 0;
  list.addCallback([&](int) { ++aCount; }, &a);
  list.addCallback([&](int) { ++bCount; }, &b);

  list.unbindAllFromEmitter(&a);
  list.triggerAll(1);

  EXPECT_EQ(aCount, 0);
  EXPECT_EQ(bCount, 1);
}

TEST(test_CallbackList, UnbindUnknownEmitterIsNoop)
{
  CallbackList<std::function<void(int)>, DummyBinder> list;
  DummyBinder a, b;
  int aCount = 0;
  list.addCallback([&](int) { ++aCount; }, &a);

  list.unbindAllFromEmitter(&b); // b was never bound
  list.triggerAll(1);

  EXPECT_EQ(aCount, 1);
}
