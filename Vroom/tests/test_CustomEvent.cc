#include <gtest/gtest.h>

#include <Vroom/Event/CustomEvent/CustomEvent.h>

class CustomEventTest : public testing::Test
{
protected:
    void SetUp() override
    {
        instance = vrm::CustomEvent();
    }

    void TearDown() override
    {

    }

    vrm::CustomEvent instance;
};

TEST_F(CustomEventTest, AddCallbackAndTriggerIt)
{
    int i = 0, j = 0;

    auto cb = [&i, &j](const vrm::Event& e)
    {
        if (e.type == vrm::Event::Type::KeyPressed)
            i++;
        else if (e.type == vrm::Event::Type::KeyReleased)
            j++;
    };

    instance.addCallback(cb);
    
    vrm::Event keyPressed, keyReleased;
    keyPressed.type = vrm::Event::Type::KeyPressed;
    keyReleased.type = vrm::Event::Type::KeyReleased;

    for (size_t k = 0; k < 1000; k++)
        instance.trigger(keyPressed);
    for (size_t k = 0; k < 2000; k++)
        instance.trigger(keyReleased);

    EXPECT_EQ(i, 1000);
    EXPECT_EQ(j, 2000);
}