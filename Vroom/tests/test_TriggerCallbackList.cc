#include <gtest/gtest.h>

#include <Vroom/Event/Trigger/TriggerCallbackList.h>

class TriggerCallbackListTest : public testing::Test
{
protected:
    void SetUp() override
    {
        instance = vrm::TriggerCallbackList();
    }

    void TearDown() override
    {

    }

    vrm::TriggerCallbackList instance;
};

TEST_F(TriggerCallbackListTest, TestNoCallback)
{
    EXPECT_NO_THROW(instance.triggerAll(true));
    EXPECT_NO_THROW(instance.triggerAll(false));
}

TEST_F(TriggerCallbackListTest, OneCallback)
{
    // Only 1 callback in the list
    
    int i = 0;
    int j = 0;
    auto incrementer = [&i, &j](bool triggered) { if (triggered) i++; else j++; };
    instance.addCallback(incrementer);

    // 3 true, 2 false
    instance.triggerAll(true);
    instance.triggerAll(true);
    instance.triggerAll(true);
    instance.triggerAll(false);
    instance.triggerAll(false);

    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 2);
}

TEST_F(TriggerCallbackListTest, MultipleCallbacks)
{
    // 10000 callbacks in the list
    
    int i = 0;
    int j = 0;
    auto incrementer = [&i, &j](bool triggered) { if (triggered) i++; else j++; };

    for (size_t k = 0; k < 10000; k++)
        instance.addCallback(incrementer);

    // 30000 true, 20000 false
    instance.triggerAll(true);
    instance.triggerAll(true);
    instance.triggerAll(true);
    instance.triggerAll(false);
    instance.triggerAll(false);

    EXPECT_EQ(i, 30000);
    EXPECT_EQ(j, 20000);
}