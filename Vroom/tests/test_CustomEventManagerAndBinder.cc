#include <gtest/gtest.h>

#include <Vroom/Event/CustomEvent/CustomEventManager.h>

class CustomEventManagerAndBinderTest : public testing::Test
{
protected:
    void SetUp() override
    {
        instance = vrm::CustomEventManager();
    }

    void TearDown() override
    {

    }

    vrm::CustomEventManager instance;
};

TEST_F(CustomEventManagerAndBinderTest, CustomEventCreation)
{
    EXPECT_NO_THROW(instance.createCustomEvent("MyCustomEvent"));
    EXPECT_NO_THROW(instance.createCustomEvent("MyCustomEvent2"));
    EXPECT_NO_THROW(instance.createCustomEvent("MyCustomEvent3"));
    EXPECT_NO_THROW(instance.createCustomEvent("MyCustomEvent4"));
}

TEST_F(CustomEventManagerAndBinderTest, MultipleCustomEventCreation)
{
    EXPECT_NO_THROW(instance.createCustomEvent("MyCustomEvent"));
    EXPECT_THROW(instance.createCustomEvent("MyCustomEvent"), std::runtime_error);
}

TEST_F(CustomEventManagerAndBinderTest, InputBinding)
{
    EXPECT_NO_THROW(
        instance.createCustomEvent("MyCustomEvent")
            .bindInput(vrm::Event::Type::Exit)
            .bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::A)
    );
}

TEST_F(CustomEventManagerAndBinderTest, MultipleInputBinding)
{
    EXPECT_THROW(
        instance.createCustomEvent("MyCustomEvent")
            .bindInput(vrm::Event::Type::Exit)
            .bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::A)
            .bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::A),
        std::runtime_error
    );
    
    EXPECT_THROW(
        instance.createCustomEvent("MyCustomEvent2")
            .bindInput(vrm::Event::Type::Exit)
            .bindInput(vrm::Event::Type::Exit)
            .bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::A),
        std::runtime_error
    );
}

TEST_F(CustomEventManagerAndBinderTest, CallbackBinding)
{
    int i = 0, j = 0, k = 0;

    EXPECT_NO_THROW(
        instance.createCustomEvent("MyCustomEvent")
            .bindInput(vrm::Event::Type::Exit)
            .bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::A)
            .bindInput(vrm::Event::Type::MouseReleased, vrm::MouseCode::Left)
            .bindCallback([&i](const vrm::Event& e) { if (e.type == vrm::Event::Type::Exit) i++; })
            .bindCallback([&j](const vrm::Event& e) { if (e.type == vrm::Event::Type::KeyPressed) j++; })
            .bindCallback([&k](const vrm::Event& e) { if (e.type == vrm::Event::Type::MouseReleased) k++; })
    );
}

TEST_F(CustomEventManagerAndBinderTest, EventChecking)
{
    int i = 0, j = 0;

    EXPECT_NO_THROW(
        instance.createCustomEvent("MyCustomEvent")
            .bindInput(vrm::Event::Type::Exit)
            .bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::A)
            .bindCallback([&i](const vrm::Event& e) { if (e.type == vrm::Event::Type::Exit) i++; })
            .bindCallback([&j](const vrm::Event& e) { if (e.type == vrm::Event::Type::KeyPressed) j++; })
    );

    vrm::Event e;
    e.type = vrm::Event::Type::Exit;
    e.standardizedCode = 0;
    instance.check(e);
    EXPECT_EQ(i, 1); // Should have called the first callback.
    EXPECT_EQ(j, 0);

    e = vrm::Event();
    e.type = vrm::Event::Type::Exit;
    e.standardizedCode = 2;
    instance.check(e);
    EXPECT_EQ(i, 2); // This shouldn't change anything and should call the first callback again because Exit was implicitely bound with code 0 (= ignores code).
    EXPECT_EQ(j, 0);

    e = vrm::Event();
    e.type = vrm::Event::Type::KeyPressed;
    e.keyCode = vrm::KeyCode::A;
    instance.check(e);
    EXPECT_EQ(i, 2);
    EXPECT_EQ(j, 1); // A key pressed was bound to manager. Second callback should be called.

    e = vrm::Event();
    e.type = vrm::Event::Type::KeyPressed;
    e.keyCode = vrm::KeyCode::U;
    instance.check(e);
    EXPECT_EQ(i, 2);
    EXPECT_EQ(j, 1); // A key was pressed, but the wrong one. No callback should have been called.

    e = vrm::Event();
    e.type = vrm::Event::Type::KeyPressed;
    e.keyCode = vrm::KeyCode::None;
    instance.check(e);
    EXPECT_EQ(i, 2);
    EXPECT_EQ(j, 1); // No callback neither. (key is none)
}