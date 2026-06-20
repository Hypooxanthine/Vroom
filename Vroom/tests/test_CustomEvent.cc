#include <gtest/gtest.h>

#include <Event/CustomEvent.h>
#include <Event/Event.h>

using namespace vrm;

TEST(test_CustomEvent, TriggerInvokesCallbackWithEvent)
{
  CustomEvent ce;

  Event received;
  received.type = Event::Type::None;
  ce.addCallback([&](const Event& e) { received = e; }, nullptr);

  Event e;
  e.type    = Event::Type::KeyPressed;
  e.keyCode = KeyCode::A;
  ce.trigger(e);

  EXPECT_EQ(received.type, Event::Type::KeyPressed);
  EXPECT_EQ(received.keyCode, KeyCode::A);
}

TEST(test_CustomEvent, MultipleCallbacksAllInvoked)
{
  CustomEvent ce;
  int calls = 0;
  ce.addCallback([&](const Event&) { ++calls; }, nullptr);
  ce.addCallback([&](const Event&) { ++calls; }, nullptr);

  Event e;
  e.type = Event::Type::Scroll;
  ce.trigger(e);

  EXPECT_EQ(calls, 2);
}

TEST(test_CustomEvent, RemoveCallbacksFromEmitter)
{
  CustomEvent ce;
  int calls = 0;
  CustomEventBinder* emitter = reinterpret_cast<CustomEventBinder*>(0x1);
  ce.addCallback([&](const Event&) { ++calls; }, emitter);

  ce.removeCallbacksFromEmitter(emitter);

  Event e;
  e.type = Event::Type::MouseMoved;
  ce.trigger(e);

  EXPECT_EQ(calls, 0);
}
