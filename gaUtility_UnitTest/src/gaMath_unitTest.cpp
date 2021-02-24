#define GTEST_HAS_TR1_TUPLE 0
#define GTEST_USE_OWN_TR1_TUPLE 0
#include <gtest/gtest.h>

#include "gaPrerequisitesUtilities.h"
#include "gaMath.h"
#include "gaDegrees.h"
#include "gaRadians.h"

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

  //EXPECT_FLOAT_EQ(Math::PI, 3.14159265f);
}

TEST(gaUtilities, Trigonometric) {
  EXPECT_FLOAT_EQ(Math::cos(0.0f), 1.0f);
  EXPECT_FLOAT_EQ(Math::sin(0.0f), 0.0f);
  //EXPECT_FLOAT_EQ(Math::tan(Math::m_pi), 0.0f);
  /***************************************************************************/
  EXPECT_FLOAT_EQ(Math::cos(90.0f), -0.4480736255645752f);
  EXPECT_FLOAT_EQ(Math::sin(90.0f), 0.89399665594100952f);
  EXPECT_FLOAT_EQ(Math::tan(45.0f), 1.6197751760482788f);
  /***************************************************************************/
  EXPECT_FLOAT_EQ(Math::arcCos(0.0f), 1.5707963705062866f);
  EXPECT_FLOAT_EQ(Math::arcSin(0.0f), 0.0f);
  //EXPECT_NEAR(Math::arcTan(Math::m_pi), 1.2626f, 0.000000001f);
  /***************************************************************************/
  EXPECT_FLOAT_EQ(Math::arcCos(1.0f), 0.0f);
  EXPECT_FLOAT_EQ(Math::arcSin(1.0f), 1.5707964f);
  EXPECT_FLOAT_EQ(Math::arcTan(1.0f), 0.78539818525314331f);
}

TEST(gaUtilities, Hyperbolic) {
  EXPECT_FLOAT_EQ(Math::cosHB(0.0f), 1.0f);
  EXPECT_FLOAT_EQ(Math::sinHB(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Math::tanHB(0.0f), 0.0f);
  /***************************************************************************/
  EXPECT_FLOAT_EQ(Math::cosHB(1.0f), 1.5430807f);
  EXPECT_FLOAT_EQ(Math::sinHB(1.0f), 1.1752012f);
  EXPECT_FLOAT_EQ(Math::tanHB(1.0f), 0.76159418f);
  /***************************************************************************/
  EXPECT_FLOAT_EQ(Math::arcCosHB(1.0f), 0.0f);
  EXPECT_FLOAT_EQ(Math::arcSinHB(1.0f), 0.88137358f);
  EXPECT_FLOAT_EQ(Math::arcTanHB(0.0f), 0.0f);
  /***************************************************************************/
  EXPECT_FLOAT_EQ(Math::arcCosHB(2.0f), 1.316958f);
  EXPECT_FLOAT_EQ(Math::arcSinHB(2.0f), 1.4436355f);
  EXPECT_FLOAT_EQ(Math::arcTanHB(0.9f), 1.4722193f);
}

TEST(gaUtilities, Exponential_And_Logarithmic_Functions) {
  EXPECT_FLOAT_EQ(Math::exp(5), 148.413159f);

  EXPECT_FLOAT_EQ(Math::log(1), 0.0f);
}

TEST(gaUtilities, Power_Functions) {
  EXPECT_FLOAT_EQ(Math::pow(2.0f, 2.0f), 4.0f);
  EXPECT_FLOAT_EQ(Math::pow(150.0f, 5.0f), 75937500000.0f);

  EXPECT_FLOAT_EQ(Math::sqrtf(15.5f), 3.9370039f);

  EXPECT_FLOAT_EQ(Math::sqrti(100.0f), 10.0f);

  EXPECT_FLOAT_EQ(Math::hypo(1.0f, 5.0f), 5.0990195f);
}

TEST(gaUtilities, Degrees_Test) {
  Degrees testDegree(150.50f);
  EXPECT_FLOAT_EQ(testDegree.degreesToRadians(testDegree.m_degree), 2.6267205f);
}

TEST(gaUtilities, Radians_Test) {
  Radians testRadians(150.50f);
  EXPECT_FLOAT_EQ(testRadians.radiansToDegrees(testRadians.m_radian), 8623.0148f);
}