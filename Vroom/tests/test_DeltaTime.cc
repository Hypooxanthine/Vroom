#include <gtest/gtest.h>
#include <chrono>

#include <Core/DeltaTime.h>

using namespace vrm;

TEST(test_DeltaTime, UnitConversions)
{
  auto dur = std::chrono::duration_cast<DeltaTime::DurationType>(std::chrono::milliseconds(5));
  DeltaTime dt(DeltaTime::ClockType::now(), dur);

  EXPECT_NEAR(dt.seconds().get<double>(), 0.005, 1e-9);
  EXPECT_NEAR(dt.milliseconds().get<double>(), 5.0, 1e-6);
  EXPECT_NEAR(dt.microseconds().get<double>(), 5000.0, 1e-3);
  EXPECT_NEAR(dt.nanoseconds().get<double>(), 5'000'000.0, 1.0);
}

TEST(test_DeltaTime, DefaultIsZeroDuration)
{
  DeltaTime dt;
  EXPECT_DOUBLE_EQ(dt.seconds().get<double>(), 0.0);
  EXPECT_DOUBLE_EQ(dt.milliseconds().get<double>(), 0.0);
}

TEST(test_DeltaTime, GetReturnsLastSet)
{
  auto dur = std::chrono::duration_cast<DeltaTime::DurationType>(std::chrono::milliseconds(10));
  DeltaTime dt(DeltaTime::ClockType::now(), dur);

  DeltaTime::Set(dt);
  EXPECT_NEAR(DeltaTime::Get().milliseconds().get<double>(), 10.0, 1e-6);
}
