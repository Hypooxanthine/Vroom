#include <gtest/gtest.h>

#include <Vroom/Event/Trigger/Trigger.h>

class TriggerTest : public testing::Test
{
protected:
    void SetUp() override
    {
        instance = vrm::Trigger();
    }

    void TearDown() override
    {

    }

    vrm::Trigger instance;
};

TEST_F(TriggerTest, IsTriggeredMethod)
{
    // At first we should'nt be triggered
    EXPECT_EQ(instance.isTriggered(), false);

    // Playing a bit
    instance.trigger(true); // 1
    EXPECT_EQ(instance.isTriggered(), true);
    instance.trigger(true); // 2
    EXPECT_EQ(instance.isTriggered(), true);
    instance.trigger(true); // 3
    EXPECT_EQ(instance.isTriggered(), true);
    instance.trigger(false); // 2
    EXPECT_EQ(instance.isTriggered(), true);
    instance.trigger(false); // 1
    EXPECT_EQ(instance.isTriggered(), true);
    instance.trigger(false); // 0
    EXPECT_EQ(instance.isTriggered(), false);

    EXPECT_NO_THROW(instance.isTriggered()); // Should be clamped to zero
    EXPECT_EQ(instance.isTriggered(), false); // Should stay not triggered

}

TEST_F(TriggerTest, TriggerAlternate)
{
    int i = 0;
    int j = 0;
    auto incrementer = [&i, &j](bool triggered) { if (triggered) i++; else j++; };
    instance.bindCallback(incrementer);
    // At each activated trigger, we should add 1 to i
    // At each deactivated trigger, we should add 1 to j

    instance.trigger(true); // Activated 1
    instance.trigger(false); // Deactivated 1
    instance.trigger(true); // Activated 2
    instance.trigger(false); // Deactivated 2
    instance.trigger(true); // Activated 3
    instance.trigger(false); // Deactivated 3
    instance.trigger(true); // Activated 4
    instance.trigger(false); // Deactivated 4
    instance.trigger(true); // Activated 5

    EXPECT_EQ(i, 5);
    EXPECT_EQ(j, 4);
}

TEST_F(TriggerTest, TriggerHighValues)
{
    int i = 0;
    int j = 0;
    auto incrementer = [&i, &j](bool triggered) { if (triggered) i++; else j++; };
    instance.bindCallback(incrementer);
    // At each activated trigger, we should add 1 to i
    // At each deactivated trigger, we should add 1 to j

    // This should activate trigger only once
    // Using uint8_t because its trigger internal storage type
    for (uint8_t k = 0; k < std::numeric_limits<uint8_t>::max(); k++)
        instance.trigger(true);
    for (uint8_t k = 0; k < std::numeric_limits<uint8_t>::max() - 1; k++)
        instance.trigger(false);

    EXPECT_EQ(i, 1);
    EXPECT_EQ(j, 0);
}