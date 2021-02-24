#define GTEST_HAS_TR1_TUPLE 0
#define GTEST_USE_OWN_TR1_TUPLE 0
#include <gtest/gtest.h>

#include "gaPrerequisitesUtilities.h"
#include "gaVector2.h"
#include "gaVector2I.h"
#include "gaVector3.h"
#include "gaVector3I.h"
#include "gaVector4.h"
#include "gaVector4I.h"

using namespace gaEngineSDK;

TEST(gaUtilities, Vector2_Tests) {
  Vector2 testVec;
  EXPECT_FLOAT_EQ(testVec.m_x, 0.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 0.0f);
  /***************************************************************************/
  testVec.m_x = 55.5f;
  testVec.m_y = 15.0f;
  EXPECT_FLOAT_EQ(testVec.m_x, 55.5f);
  EXPECT_FLOAT_EQ(testVec.m_y, 15.0f);
  /***************************************************************************/
  EXPECT_FLOAT_EQ(testVec.magnitude(), 57.4913025f);
  /***************************************************************************/
  testVec.normalize();
  EXPECT_FLOAT_EQ(testVec.m_x, 0.965363443f);
  EXPECT_FLOAT_EQ(testVec.m_y, 0.260909021f);
  /***************************************************************************/
  testVec.m_x = 50.0f;
  testVec.m_y = 20.0f;
  EXPECT_FLOAT_EQ(testVec.dotProduct(testVec), 2900.0f);
  /***************************************************************************/
  Vector2 testVec2(150.0f, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.m_x, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 150.0f);

  EXPECT_TRUE(testVec   <  testVec2);
  EXPECT_TRUE(testVec2  >  testVec);

  EXPECT_FALSE(testVec  >  testVec2);
  EXPECT_FALSE(testVec  >= testVec2);
  EXPECT_FALSE(testVec2 <  testVec);
  EXPECT_FALSE(testVec2 <= testVec);

  testVec = testVec2;
  EXPECT_TRUE(testVec == testVec2);
  EXPECT_TRUE(testVec2 == testVec);

  testVec = testVec + testVec;
  EXPECT_FLOAT_EQ(testVec.m_x, 300.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 300.0f);

  testVec2 += testVec;
  EXPECT_FLOAT_EQ(testVec2.m_x, 450.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 450.0f);

  testVec = testVec2 - testVec;
  EXPECT_FLOAT_EQ(testVec.m_x, 150.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 150.0f);

  testVec2 -= testVec;
  EXPECT_FLOAT_EQ(testVec2.m_x, 300.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 300.0f);

  testVec = testVec * testVec;
  EXPECT_FLOAT_EQ(testVec.m_x, 22500.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 22500.0f);

  testVec2 *= 6.0f;
  EXPECT_FLOAT_EQ(testVec2.m_x, 1800.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 1800.0f);

  testVec = testVec / 300.0f;
  EXPECT_FLOAT_EQ(testVec.m_x, 75.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 75.0f);

  testVec2 /= 15.0f;
  EXPECT_FLOAT_EQ(testVec2.m_x, 120.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 120.0f);

  testVec2 = testVec * 2.0f;
  EXPECT_FLOAT_EQ(testVec2.m_x, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 150.0f);
}

TEST(gaUtilities, Vector2I_Tests) {
  Vector2I testVec;
  EXPECT_EQ(testVec.m_x, 0);
  EXPECT_EQ(testVec.m_y, 0);
  /***************************************************************************/
  testVec.m_x = 55;
  testVec.m_y = 15;
  EXPECT_EQ(testVec.m_x, 55);
  EXPECT_EQ(testVec.m_y, 15);
  /***************************************************************************/
  EXPECT_EQ(testVec.magnitude(), 57);
  /***************************************************************************/
  testVec.m_x = 50;
  testVec.m_y = 20;
  EXPECT_EQ(testVec.dotProduct(testVec), 2900);
  /***************************************************************************/
  Vector2I testVec2(150, 150);
  EXPECT_EQ(testVec2.m_x, 150);
  EXPECT_EQ(testVec2.m_y, 150);
  
  EXPECT_TRUE(testVec  < testVec2);
  EXPECT_TRUE(testVec2 > testVec);
  
  EXPECT_FALSE(testVec  >  testVec2);
  EXPECT_FALSE(testVec  >= testVec2);
  EXPECT_FALSE(testVec2 <  testVec);
  EXPECT_FALSE(testVec2 <= testVec);
  
  testVec = testVec2;
  EXPECT_TRUE(testVec  == testVec2);
  EXPECT_TRUE(testVec2 == testVec);
  
  testVec = testVec + testVec;
  EXPECT_EQ(testVec.m_x, 300);
  EXPECT_EQ(testVec.m_y, 300);
  
  testVec2 += testVec;
  EXPECT_EQ(testVec2.m_x, 450);
  EXPECT_EQ(testVec2.m_y, 450);
  
  testVec = testVec2 - testVec;
  EXPECT_EQ(testVec.m_x, 150);
  EXPECT_EQ(testVec.m_y, 150);
  
  testVec2 -= testVec;
  EXPECT_EQ(testVec2.m_x, 300);
  EXPECT_EQ(testVec2.m_y, 300);
  
  testVec = testVec * testVec;
  EXPECT_EQ(testVec.m_x, 22500);
  EXPECT_EQ(testVec.m_y, 22500);
  
  testVec2 *= 6;
  EXPECT_EQ(testVec2.m_x, 1800);
  EXPECT_EQ(testVec2.m_y, 1800);
  
  testVec = testVec / 300;
  EXPECT_EQ(testVec.m_x, 75);
  EXPECT_EQ(testVec.m_y, 75);
  
  testVec2 /= 15;
  EXPECT_EQ(testVec2.m_x, 120);
  EXPECT_EQ(testVec2.m_y, 120);
  
  testVec2 = testVec * 2;
  EXPECT_EQ(testVec2.m_x, 150);
  EXPECT_EQ(testVec2.m_y, 150);
}

//TODO: verificar por que no detecta el vector 3
TEST(gaUtilities, Vector3_Tests) {
  Vector3 testVec;
  EXPECT_FLOAT_EQ(testVec.m_x, 0.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 0.0f);
  EXPECT_FLOAT_EQ(testVec.m_z, 0.0f);
  /***************************************************************************/
  testVec.m_x = 100.0f;
  testVec.m_y = 20.0f;
  testVec.m_z = 40.0f;
  EXPECT_FLOAT_EQ(testVec.m_x, 100.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 20.0f);
  EXPECT_FLOAT_EQ(testVec.m_z, 40.0f);
  /***************************************************************************/
  EXPECT_FLOAT_EQ(testVec.magnitude(), 109.544511f);
  /***************************************************************************/
  testVec.normalize();
  EXPECT_FLOAT_EQ(testVec.m_x, 100.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 100.0f);
  EXPECT_FLOAT_EQ(testVec.m_z, 100.0f);
  /***************************************************************************/
  testVec.m_x = 50.0f;
  testVec.m_y = 20.0f;
  testVec.m_z = 40.0f;
  EXPECT_FLOAT_EQ(testVec.dotProduct(testVec), 2900.0f);
  /***************************************************************************/
  Vector3 testVec2(150.0f, 150.0f, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.m_x, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.m_z, 150.0f);

  EXPECT_TRUE(testVec  < testVec2);
  EXPECT_TRUE(testVec2 > testVec);

  EXPECT_FALSE(testVec  >  testVec2);
  EXPECT_FALSE(testVec  >= testVec2);
  EXPECT_FALSE(testVec2 <  testVec);
  EXPECT_FALSE(testVec2 <= testVec);

  testVec = testVec2;
  EXPECT_TRUE(testVec  == testVec2);
  EXPECT_TRUE(testVec2 == testVec);

  testVec = testVec + testVec;
  EXPECT_FLOAT_EQ(testVec.m_x, 300.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 300.0f);
  EXPECT_FLOAT_EQ(testVec.m_z, 300.0f);

  testVec2 += testVec;
  EXPECT_FLOAT_EQ(testVec2.m_x, 450.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 450.0f);
  EXPECT_FLOAT_EQ(testVec2.m_z, 450.0f);

  testVec = testVec2 - testVec;
  EXPECT_FLOAT_EQ(testVec.m_x, 150.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 150.0f);
  EXPECT_FLOAT_EQ(testVec.m_z, 150.0f);

  testVec2 -= testVec;
  EXPECT_FLOAT_EQ(testVec2.m_x, 300.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 300.0f);
  EXPECT_FLOAT_EQ(testVec2.m_z, 300.0f);

  testVec = testVec * testVec;
  EXPECT_FLOAT_EQ(testVec.m_x, 22500.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 22500.0f);
  EXPECT_FLOAT_EQ(testVec.m_z, 22500.0f);

  testVec2 *= 6.0f;
  EXPECT_FLOAT_EQ(testVec2.m_x, 1800.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 1800.0f);
  EXPECT_FLOAT_EQ(testVec2.m_z, 1800.0f);

  testVec = testVec / 300.0f;
  EXPECT_FLOAT_EQ(testVec.m_x, 75.0f);
  EXPECT_FLOAT_EQ(testVec.m_y, 75.0f);
  EXPECT_FLOAT_EQ(testVec.m_z, 75.0f);

  testVec2 /= 15.0f;
  EXPECT_FLOAT_EQ(testVec2.m_x, 120.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 120.0f);
  EXPECT_FLOAT_EQ(testVec2.m_z, 120.0f);

  testVec2 = testVec * 2.0f;
  EXPECT_FLOAT_EQ(testVec2.m_x, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.m_y, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.m_z, 150.0f);
}

