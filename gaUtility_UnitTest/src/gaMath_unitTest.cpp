#define GTEST_HAS_TR1_TUPLE 0
#define GTEST_USE_OWN_TR1_TUPLE 0
#include <gtest/gtest.h>

#include <gaPrerequisitesUtilities.h>
#include <gaMath.h>
#include <gaDegrees.h>
#include <gaRadians.h>

using namespace gaEngineSDK;

TEST(gaUtilities, Type_Size) {
  EXPECT_EQ(sizeof(uint8), 1U);
  EXPECT_EQ(sizeof(uint16), 2U);
  EXPECT_EQ(sizeof(uint32), 4U);
  EXPECT_EQ(sizeof(uint64), 8U);
  /***************************************************************************/
  EXPECT_EQ(sizeof(int8), 1);
  EXPECT_EQ(sizeof(int16), 2);
  EXPECT_EQ(sizeof(int32), 4);
  EXPECT_EQ(sizeof(int64), 8);
  /***************************************************************************/
  EXPECT_EQ(sizeof(char), 1);
  EXPECT_EQ(sizeof(float), 4);
  EXPECT_EQ(sizeof(double), 8);
  EXPECT_EQ(sizeof(unsigned char), 1U);

  EXPECT_FLOAT_EQ(Math::PI, 3.14159265f);
}

TEST(gaUtilities, Trigonometric) {
  EXPECT_FLOAT_EQ(Math::cos(0.0f), 1.0f);
  EXPECT_FLOAT_EQ(Math::sin(0.0f), 0.0f);
  EXPECT_NEAR(Math::tan(Math::PI), 8.742277f, Math::BIG_NUMBER);
  /***************************************************************************/
  EXPECT_NEAR(Math::cos(90.0f), -0.448073f, Math::SMALL_NUMBER);
  EXPECT_NEAR(Math::sin(90.0f), 0.893996f, Math::SMALL_NUMBER);
  EXPECT_NEAR(Math::tan(45.0f), 1.619775f, Math::SMALL_NUMBER);
  /***************************************************************************/
  EXPECT_NEAR(Math::arcCos(0.0f), 1.570796f, Math::SMALL_NUMBER);
  EXPECT_FLOAT_EQ(Math::arcSin(0.0f), 0.0f);
  EXPECT_NEAR(Math::arcTan(Math::PI), 1.2626f, Math::BIG_NUMBER);
  /***************************************************************************/
  EXPECT_FLOAT_EQ(Math::arcCos(1.0f), 0.0f);
  EXPECT_NEAR(Math::arcSin(1.0f), 1.570796f, Math::SMALL_NUMBER);
  EXPECT_NEAR(Math::arcTan(1.0f), 0.785398f, Math::SMALL_NUMBER);
}

TEST(gaUtilities, Hyperbolic) {
  EXPECT_FLOAT_EQ(Math::cosHB(0.0f), 1.0f);
  EXPECT_FLOAT_EQ(Math::sinHB(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Math::tanHB(0.0f), 0.0f);
  /***************************************************************************/
  EXPECT_NEAR(Math::cosHB(1.0f), 1.543080f, Math::SMALL_NUMBER);
  EXPECT_NEAR(Math::sinHB(1.0f), 1.175201f, Math::SMALL_NUMBER);
  EXPECT_NEAR(Math::tanHB(1.0f), 0.761594f, Math::SMALL_NUMBER);
  /***************************************************************************/
  EXPECT_FLOAT_EQ(Math::arcCosHB(1.0f), 0.0f);
  EXPECT_NEAR(Math::arcSinHB(1.0f), 0.881373f, Math::SMALL_NUMBER);
  EXPECT_FLOAT_EQ(Math::arcTanHB(0.0f), 0.0f);
  /***************************************************************************/
  EXPECT_NEAR(Math::arcCosHB(2.0f), 1.316958f, Math::SMALL_NUMBER);
  EXPECT_NEAR(Math::arcSinHB(2.0f), 1.443635f, Math::SMALL_NUMBER);
  EXPECT_NEAR(Math::arcTanHB(0.9f), 1.472219f, Math::SMALL_NUMBER);
}

TEST(gaUtilities, Exponential_And_Logarithmic_Functions) {
  EXPECT_FLOAT_EQ(Math::exp(5), 148.413159f);
  EXPECT_FLOAT_EQ(Math::log(1), 0.0f);
}

TEST(gaUtilities, Power_Functions) {
  EXPECT_FLOAT_EQ(Math::pow(2.0f, 2.0f), 4.0f);
  EXPECT_FLOAT_EQ(Math::pow(150.0f, 5.0f), 75937500000.0f);

  EXPECT_NEAR(Math::sqrtf(15.5f), 3.937003f, Math::SMALL_NUMBER);
  EXPECT_NEAR(Math::hypo(1.0f, 5.0f), 5.099019f, Math::SMALL_NUMBER);
}

TEST(gaUtilities, Radians_Degrees_Test) {
  Radians r = Radians(1);
  Degrees d = Degrees(180);

  EXPECT_FLOAT_EQ(r.valueDegrees(), 57.295776f);
  EXPECT_FLOAT_EQ(r.valueRadians(), 1.f);
  EXPECT_FLOAT_EQ(d.valueRadians(), Math::PI);

  GTEST_ASSERT_EQ(r == d, false);
  GTEST_ASSERT_EQ(r != d, true);
  GTEST_ASSERT_EQ(r < d, true);
  GTEST_ASSERT_EQ(r > d, false);

  r = d;

  GTEST_ASSERT_EQ(r == d, true);
  GTEST_ASSERT_EQ(r != d, false);
  GTEST_ASSERT_EQ(r < d, false);
  GTEST_ASSERT_EQ(r > d, false);

  r = 2.f;
  d = 2.f;

  EXPECT_FLOAT_EQ(r.valueRadians(), 2.f);
}