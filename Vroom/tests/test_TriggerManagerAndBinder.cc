#include <gtest/gtest.h>

#include <Vroom/Event/Trigger/TriggerManager.h>

class TriggerManagerAndBinderTest : public testing::Test
{
protected:
    void SetUp() override
    {
        instance = vrm::TriggerManager();
    }

    void TearDown() override
    {

    }

    vrm::TriggerManager instance;
};

TEST_F(TriggerManagerAndBinderTest, TriggerCreation)
{
    EXPECT_NO_THROW(instance.createTrigger("MyTrigger"));
    EXPECT_NO_THROW(instance.createTrigger("MyTrigger2"));
    EXPECT_NO_THROW(instance.createTrigger("MyTrigger3"));
    EXPECT_NO_THROW(instance.createTrigger("MyTrigger4"));
    EXPECT_THROW(instance.createTrigger("MyTrigger"), std::runtime_error);
}

TEST_F(TriggerManagerAndBinderTest, InputBinding)
{
    EXPECT_NO_THROW(
        instance.createTrigger("MyTrigger")
            .bindInput(vrm::KeyCode::A)
            .bindInput(vrm::MouseCode::MB4)
    );

}

TEST_F(TriggerManagerAndBinderTest, DoubleInputBinding)
{
    EXPECT_THROW(
        instance.createTrigger("MyTrigger")
            .bindInput(vrm::KeyCode::Q)
            .bindInput(vrm::KeyCode::Q),
        std::runtime_error
    );
    EXPECT_THROW(
        instance.createTrigger("OtherTrigger")
            .bindInput(vrm::MouseCode::Left)
            .bindInput(vrm::MouseCode::Left),
        std::runtime_error
    );

}

TEST_F(TriggerManagerAndBinderTest, CallbackBinding)
{
    int i = 0, j = 0;
    EXPECT_NO_THROW(
        instance.createTrigger("MyTrigger")
            .bindCallback([&i](bool activated) { i++; })
            .bindCallback([&j](bool activated) { j++; })
    );
}

TEST_F(TriggerManagerAndBinderTest, EventChecking)
{
    int i = 0, j = 0;
    EXPECT_NO_THROW(
        instance.createTrigger("MyTrigger")
            .bindInput(vrm::KeyCode::A)
            .bindInput(vrm::MouseCode::MB4)
            .bindCallback([&i](bool activated) { if (activated) i++; })
            .bindCallback([&j](bool activated) { if (!activated) j++; })
    );

    vrm::Event e;
    e.type = vrm::Event::Type::KeyPressed;
    e.keyCode = vrm::KeyCode::A;
    EXPECT_NO_THROW(instance.check(e));
    EXPECT_EQ(i, 1);
    EXPECT_EQ(j, 0);

    e.keyCode = vrm::KeyCode::Z;
    EXPECT_NO_THROW(instance.check(e)); // Nothing should change
    EXPECT_EQ(i, 1);
    EXPECT_EQ(j, 0);
    
    e.type = vrm::Event::Type::KeyReleased;
    e.keyCode = vrm::KeyCode::A;
    EXPECT_NO_THROW(instance.check(e));
    EXPECT_EQ(i, 1);
    EXPECT_EQ(j, 1);

}