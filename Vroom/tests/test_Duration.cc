#include <gtest/gtest.h>

#include <Core/Duration.h>

using namespace vrm;

TEST(test_Duration, ConversionByDivisor)
{
  EXPECT_DOUBLE_EQ(DurationSeconds(1'000'000'000ull).get<double>(), 1.0);
  EXPECT_DOUBLE_EQ(DurationMilliseconds(1'000'000ull).get<double>(), 1.0);
  EXPECT_DOUBLE_EQ(DurationMicroseconds(1'000ull).get<double>(), 1.0);
  EXPECT_DOUBLE_EQ(DurationNanoseconds(1ull).get<double>(), 1.0);
}

TEST(test_Duration, SameNanosecondsDifferentUnits)
{
  // 1.5 seconds expressed in nanoseconds.
  const uint64_t ns = 1'500'000'000ull;

  EXPECT_DOUBLE_EQ(DurationSeconds(ns).get<double>(), 1.5);
  EXPECT_DOUBLE_EQ(DurationMilliseconds(ns).get<double>(), 1500.0);
  EXPECT_DOUBLE_EQ(DurationNanoseconds(ns).get<double>(), 1'500'000'000.0);
}

TEST(test_Duration, ImplicitConversion)
{
  double d = DurationSeconds(2'000'000'000ull);
  EXPECT_DOUBLE_EQ(d, 2.0);
}

TEST(test_Duration, ArithmeticOperators)
{
  double base = 10.0;
  base += DurationSeconds(5'000'000'000ull); // + 5 s
  EXPECT_DOUBLE_EQ(base, 15.0);

  EXPECT_DOUBLE_EQ(10.0 - DurationSeconds(3'000'000'000ull), 7.0);
  EXPECT_DOUBLE_EQ(2.0 * DurationSeconds(3'000'000'000ull), 6.0);
  EXPECT_DOUBLE_EQ(DurationSeconds(6'000'000'000ull) / 2.0, 3.0);
}
