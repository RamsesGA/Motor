#define GTEST_HAS_TR1_TUPLE 0
#define GTEST_USE_OWN_TR1_TUPLE 0
#include <gtest/gtest.h>

#include <gaPrerequisitesUtilities.h>
#include <gaVector2.h>
#include <gaVector2I.h>
#include <gaVector3.h>
#include <gaVector3I.h>
#include <gaVector4.h>
#include <gaVector4I.h>

using namespace gaEngineSDK;

TEST(gaUtilities, Vector2_Tests) {
  Vector2 testVec;
  EXPECT_FLOAT_EQ(testVec.x, 0.0f);
  EXPECT_FLOAT_EQ(testVec.y, 0.0f);
  /***************************************************************************/
  testVec.x = 55.5f;
  testVec.y = 15.0f;
  EXPECT_FLOAT_EQ(testVec.x, 55.5f);
  EXPECT_FLOAT_EQ(testVec.y, 15.0f);
  /***************************************************************************/
  EXPECT_NEAR(testVec.magnitude(), 57.491302f, Math::SMALL_NUMBER);
  /***************************************************************************/
  testVec.normalize();
  EXPECT_NEAR(testVec.x, 0.965363f, Math::SMALL_NUMBER);
  EXPECT_NEAR(testVec.y, 0.260909f, Math::SMALL_NUMBER);
  /***************************************************************************/
  testVec.x = 50.0f;
  testVec.y = 20.0f;
  EXPECT_FLOAT_EQ(testVec.dotProduct(testVec), 2900.0f);
  /***************************************************************************/
  Vector2 testVec2(150.0f, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.x, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 150.0f);

  EXPECT_TRUE(testVec   <  testVec2);
  EXPECT_TRUE(testVec2  >  testVec);

  EXPECT_FALSE(testVec  >  testVec2);
  EXPECT_FALSE(testVec2 <  testVec);

  testVec = testVec2;
  EXPECT_TRUE(testVec == testVec2);
  EXPECT_TRUE(testVec2 == testVec);

  testVec = testVec + testVec;
  EXPECT_FLOAT_EQ(testVec.x, 300.0f);
  EXPECT_FLOAT_EQ(testVec.y, 300.0f);

  testVec2 += testVec;
  EXPECT_FLOAT_EQ(testVec2.x, 450.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 450.0f);

  testVec = testVec2 - testVec;
  EXPECT_FLOAT_EQ(testVec.x, 150.0f);
  EXPECT_FLOAT_EQ(testVec.y, 150.0f);

  testVec2 -= testVec;
  EXPECT_FLOAT_EQ(testVec2.x, 300.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 300.0f);

  testVec = testVec * testVec;
  EXPECT_FLOAT_EQ(testVec.x, 22500.0f);
  EXPECT_FLOAT_EQ(testVec.y, 22500.0f);

  testVec2 *= 6.0f;
  EXPECT_FLOAT_EQ(testVec2.x, 1800.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 1800.0f);

  testVec2 = testVec * 2.0f;
  EXPECT_FLOAT_EQ(testVec2.x, 45000.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 45000.0f);
}

TEST(gaUtilities, Vector2I_Tests) {
  Vector2I testVec;
  EXPECT_EQ(testVec.x, 0);
  EXPECT_EQ(testVec.y, 0);
  /***************************************************************************/
  testVec.x = 55;
  testVec.y = 15;
  EXPECT_EQ(testVec.x, 55);
  EXPECT_EQ(testVec.y, 15);
  /***************************************************************************/
  testVec.x = 50;
  testVec.y = 20;
  EXPECT_EQ(testVec.dotProduct(testVec), 2900);
  /***************************************************************************/
  Vector2I testVec2(150, 150);
  EXPECT_EQ(testVec2.x, 150);
  EXPECT_EQ(testVec2.y, 150);
  
  EXPECT_TRUE(testVec  < testVec2);
  EXPECT_TRUE(testVec2 > testVec);
  
  EXPECT_FALSE(testVec  >  testVec2);
  EXPECT_FALSE(testVec2 <  testVec);
  
  testVec = testVec2;
  EXPECT_TRUE(testVec  == testVec2);
  EXPECT_TRUE(testVec2 == testVec);
  
  testVec = testVec + testVec;
  EXPECT_EQ(testVec.x, 300);
  EXPECT_EQ(testVec.y, 300);
  
  testVec2 += testVec;
  EXPECT_EQ(testVec2.x, 450);
  EXPECT_EQ(testVec2.y, 450);
  
  testVec = testVec2 - testVec;
  EXPECT_EQ(testVec.x, 150);
  EXPECT_EQ(testVec.y, 150);
  
  testVec2 -= testVec;
  EXPECT_EQ(testVec2.x, 300);
  EXPECT_EQ(testVec2.y, 300);
  
  testVec = testVec * testVec;
  EXPECT_EQ(testVec.x, 22500);
  EXPECT_EQ(testVec.y, 22500);
  
  testVec2 *= 6;
  EXPECT_EQ(testVec2.x, 1800);
  EXPECT_EQ(testVec2.y, 1800);
  
  testVec2 = testVec * 2;
  EXPECT_EQ(testVec2.x, 45000);
  EXPECT_EQ(testVec2.y, 45000);
}

TEST(gaUtilities, Vector3_Tests) {
  Vector3 testVec;
  EXPECT_FLOAT_EQ(testVec.x, 0.0f);
  EXPECT_FLOAT_EQ(testVec.y, 0.0f);
  EXPECT_FLOAT_EQ(testVec.z, 0.0f);
  /***************************************************************************/
  testVec.x = 100.0f;
  testVec.y = 20.0f;
  testVec.z = 40.0f;
  EXPECT_FLOAT_EQ(testVec.x, 100.0f);
  EXPECT_FLOAT_EQ(testVec.y, 20.0f);
  EXPECT_FLOAT_EQ(testVec.z, 40.0f);
  /***************************************************************************/
  EXPECT_NEAR(testVec.magnitude(), 109.544510f, Math::SMALL_NUMBER);
  /***************************************************************************/
  testVec.normalize();
  EXPECT_NEAR(testVec.x, 0.912870f, Math::SMALL_NUMBER);
  EXPECT_NEAR(testVec.y, 0.182574f, Math::SMALL_NUMBER);
  EXPECT_NEAR(testVec.z, 0.365148f, Math::SMALL_NUMBER);
  /***************************************************************************/
  testVec.x = 50.0f;
  testVec.y = 20.0f;
  testVec.z = 40.0f;
  EXPECT_FLOAT_EQ(testVec.dotProduct(testVec), 4500.0f);
  /***************************************************************************/
  Vector3 testVec2(150.0f, 150.0f, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.x, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.z, 150.0f);

  EXPECT_TRUE(testVec  < testVec2);
  EXPECT_TRUE(testVec2 > testVec);

  EXPECT_FALSE(testVec  >  testVec2);
  EXPECT_FALSE(testVec2 <  testVec);

  testVec = testVec2;
  EXPECT_TRUE(testVec  == testVec2);
  EXPECT_TRUE(testVec2 == testVec);

  testVec = testVec + testVec;
  EXPECT_FLOAT_EQ(testVec.x, 300.0f);
  EXPECT_FLOAT_EQ(testVec.y, 300.0f);
  EXPECT_FLOAT_EQ(testVec.z, 300.0f);

  testVec2 += testVec;
  EXPECT_FLOAT_EQ(testVec2.x, 450.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 450.0f);
  EXPECT_FLOAT_EQ(testVec2.z, 450.0f);

  testVec = testVec2 - testVec;
  EXPECT_FLOAT_EQ(testVec.x, 150.0f);
  EXPECT_FLOAT_EQ(testVec.y, 150.0f);
  EXPECT_FLOAT_EQ(testVec.z, 150.0f);

  testVec2 -= testVec;
  EXPECT_FLOAT_EQ(testVec2.x, 300.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 300.0f);
  EXPECT_FLOAT_EQ(testVec2.z, 300.0f);

  testVec = testVec * testVec;
  EXPECT_FLOAT_EQ(testVec.x, 22500.0f);
  EXPECT_FLOAT_EQ(testVec.y, 22500.0f);
  EXPECT_FLOAT_EQ(testVec.z, 22500.0f);

  testVec2 *= 6.0f;
  EXPECT_FLOAT_EQ(testVec2.x, 1800.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 1800.0f);
  EXPECT_FLOAT_EQ(testVec2.z, 1800.0f);

  testVec2 = testVec * 2.0f;
  EXPECT_FLOAT_EQ(testVec2.x, 45000.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 45000.0f);
  EXPECT_FLOAT_EQ(testVec2.z, 45000.0f);
}

TEST(gaUtilities, Vector3I_Tests) {
  Vector3I testVec;
  EXPECT_EQ(testVec.x, 0);
  EXPECT_EQ(testVec.y, 0);
  EXPECT_EQ(testVec.z, 0);
  /***************************************************************************/
  testVec.x = 55;
  testVec.y = 15;
  testVec.z = 20;
  EXPECT_EQ(testVec.x, 55);
  EXPECT_EQ(testVec.y, 15);
  EXPECT_EQ(testVec.z, 20);
  /***************************************************************************/
  testVec.x = 50;
  testVec.y = 20;
  testVec.y = 50;
  EXPECT_EQ(testVec.dotProduct(testVec), 5400);
  /***************************************************************************/
  Vector3I testVec2(150, 150, 150);
  EXPECT_EQ(testVec2.x, 150);
  EXPECT_EQ(testVec2.y, 150);
  EXPECT_EQ(testVec2.z, 150);

  EXPECT_TRUE(testVec  < testVec2);
  EXPECT_TRUE(testVec2 > testVec);

  EXPECT_FALSE(testVec  >  testVec2);
  EXPECT_FALSE(testVec2 <  testVec);

  testVec = testVec2;
  EXPECT_TRUE(testVec  == testVec2);
  EXPECT_TRUE(testVec2 == testVec);

  testVec = testVec + testVec;
  EXPECT_EQ(testVec.x, 300);
  EXPECT_EQ(testVec.y, 300);
  EXPECT_EQ(testVec.z, 300);

  testVec2 += testVec;
  EXPECT_EQ(testVec2.x, 450);
  EXPECT_EQ(testVec2.y, 450);
  EXPECT_EQ(testVec2.z, 450);

  testVec = testVec2 - testVec;
  EXPECT_EQ(testVec.x, 150);
  EXPECT_EQ(testVec.y, 150);
  EXPECT_EQ(testVec.z, 150);

  testVec2 -= testVec;
  EXPECT_EQ(testVec2.x, 300);
  EXPECT_EQ(testVec2.y, 300);
  EXPECT_EQ(testVec2.z, 300);

  testVec = testVec * testVec;
  EXPECT_EQ(testVec.x, 22500);
  EXPECT_EQ(testVec.y, 22500);
  EXPECT_EQ(testVec.z, 22500);

  testVec2 *= 6;
  EXPECT_EQ(testVec2.x, 1800);
  EXPECT_EQ(testVec2.y, 1800);
  EXPECT_EQ(testVec2.z, 1800);

  testVec2 = testVec * 2;
  EXPECT_EQ(testVec2.x, 45000);
  EXPECT_EQ(testVec2.y, 45000);
  EXPECT_EQ(testVec2.z, 45000);
}

TEST(gaUtilities, Vector4_Tests) {
  Vector4 testVec;
  EXPECT_FLOAT_EQ(testVec.x, 0.0f);
  EXPECT_FLOAT_EQ(testVec.y, 0.0f);
  EXPECT_FLOAT_EQ(testVec.z, 0.0f);
  EXPECT_FLOAT_EQ(testVec.w, 1.0f);
  /***************************************************************************/
  testVec.x = 100.0f;
  testVec.y = 20.0f;
  testVec.z = 40.0f;
  testVec.w = 80.0f;
  EXPECT_FLOAT_EQ(testVec.x, 100.0f);
  EXPECT_FLOAT_EQ(testVec.y, 20.0f);
  EXPECT_FLOAT_EQ(testVec.z, 40.0f);
  EXPECT_FLOAT_EQ(testVec.w, 80.0f);
  /***************************************************************************/
  EXPECT_NEAR(testVec.magnitude(), 135.64661f, Math::SMALL_NUMBER);
  /***************************************************************************/
  testVec.normalize();
  EXPECT_NEAR(testVec.x, 0.7372097f, Math::SMALL_NUMBER);
  EXPECT_NEAR(testVec.y, 0.1474419f, Math::SMALL_NUMBER);
  EXPECT_NEAR(testVec.z, 0.2948839f, Math::SMALL_NUMBER);
  EXPECT_NEAR(testVec.w, 0.5897678f, Math::SMALL_NUMBER);
  /***************************************************************************/
  testVec.x = 50.0f;
  testVec.y = 20.0f;
  testVec.z = 40.0f;
  testVec.z = 140.0f;
  EXPECT_NEAR(testVec.dotProduct(testVec), 22500.348f, Math::SMALL_NUMBER);
  /***************************************************************************/
  Vector4 testVec2(150.0f, 150.0f, 150.0f, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.x, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.z, 150.0f);
  EXPECT_FLOAT_EQ(testVec2.w, 150.0f);

  EXPECT_TRUE(testVec  < testVec2);
  EXPECT_TRUE(testVec2 > testVec);

  EXPECT_FALSE(testVec > testVec2);
  EXPECT_FALSE(testVec2 < testVec);

  testVec = testVec2;
  EXPECT_TRUE(testVec == testVec2);
  EXPECT_TRUE(testVec2 == testVec);

  testVec = testVec + testVec;
  EXPECT_FLOAT_EQ(testVec.x, 300.0f);
  EXPECT_FLOAT_EQ(testVec.y, 300.0f);
  EXPECT_FLOAT_EQ(testVec.z, 300.0f);
  EXPECT_FLOAT_EQ(testVec.w, 300.0f);

  testVec2 += testVec;
  EXPECT_FLOAT_EQ(testVec2.x, 450.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 450.0f);
  EXPECT_FLOAT_EQ(testVec2.z, 450.0f);
  EXPECT_FLOAT_EQ(testVec2.w, 450.0f);

  testVec = testVec2 - testVec;
  EXPECT_FLOAT_EQ(testVec.x, 150.0f);
  EXPECT_FLOAT_EQ(testVec.y, 150.0f);
  EXPECT_FLOAT_EQ(testVec.z, 150.0f);
  EXPECT_FLOAT_EQ(testVec.w, 150.0f);

  testVec2 -= testVec;
  EXPECT_FLOAT_EQ(testVec2.x, 300.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 300.0f);
  EXPECT_FLOAT_EQ(testVec2.z, 300.0f);
  EXPECT_FLOAT_EQ(testVec2.w, 300.0f);

  testVec = testVec * testVec;
  EXPECT_FLOAT_EQ(testVec.x, 22500.0f);
  EXPECT_FLOAT_EQ(testVec.y, 22500.0f);
  EXPECT_FLOAT_EQ(testVec.z, 22500.0f);
  EXPECT_FLOAT_EQ(testVec.w, 22500.0f);

  testVec2 *= 6.0f;
  EXPECT_FLOAT_EQ(testVec2.x, 1800.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 1800.0f);
  EXPECT_FLOAT_EQ(testVec2.z, 1800.0f);
  EXPECT_FLOAT_EQ(testVec2.w, 1800.0f);

  testVec2 = testVec * 2.0f;
  EXPECT_FLOAT_EQ(testVec2.x, 45000.0f);
  EXPECT_FLOAT_EQ(testVec2.y, 45000.0f);
  EXPECT_FLOAT_EQ(testVec2.z, 45000.0f);
  EXPECT_FLOAT_EQ(testVec2.w, 45000.0f);
}

TEST(gaUtilities, Vector4I_Tests) {
  Vector4I testVec;
  EXPECT_EQ(testVec.x, 0);
  EXPECT_EQ(testVec.y, 0);
  EXPECT_EQ(testVec.z, 0);
  EXPECT_EQ(testVec.w, 1);
  /***************************************************************************/
  testVec.x = 55;
  testVec.y = 15;
  testVec.z = 20;
  testVec.w = 150;
  EXPECT_EQ(testVec.x, 55);
  EXPECT_EQ(testVec.y, 15);
  EXPECT_EQ(testVec.z, 20);
  EXPECT_EQ(testVec.w, 150);
  /***************************************************************************/
  testVec.x = 50;
  testVec.y = 20;
  testVec.y = 50;
  testVec.w = 80;
  EXPECT_EQ(testVec.dotProduct(testVec), 11800);
  /***************************************************************************/
  Vector4I testVec2(150, 150, 150, 150);
  EXPECT_EQ(testVec2.x, 150);
  EXPECT_EQ(testVec2.y, 150);
  EXPECT_EQ(testVec2.z, 150);
  EXPECT_EQ(testVec2.w, 150);

  EXPECT_TRUE(testVec  < testVec2);
  EXPECT_TRUE(testVec2 > testVec);

  EXPECT_FALSE(testVec  >  testVec2);
  EXPECT_FALSE(testVec2 <  testVec);

  testVec = testVec2;
  EXPECT_TRUE(testVec  == testVec2);
  EXPECT_TRUE(testVec2 == testVec);

  testVec = testVec + testVec;
  EXPECT_EQ(testVec.x, 300);
  EXPECT_EQ(testVec.y, 300);
  EXPECT_EQ(testVec.z, 300);
  EXPECT_EQ(testVec.w, 300);

  testVec2 += testVec;
  EXPECT_EQ(testVec2.x, 450);
  EXPECT_EQ(testVec2.y, 450);
  EXPECT_EQ(testVec2.z, 450);
  EXPECT_EQ(testVec2.w, 450);

  testVec = testVec2 - testVec;
  EXPECT_EQ(testVec.x, 150);
  EXPECT_EQ(testVec.y, 150);
  EXPECT_EQ(testVec.z, 150);
  EXPECT_EQ(testVec.w, 150);

  testVec2 -= testVec;
  EXPECT_EQ(testVec2.x, 300);
  EXPECT_EQ(testVec2.y, 300);
  EXPECT_EQ(testVec2.z, 300);
  EXPECT_EQ(testVec2.w, 300);

  testVec = testVec * testVec;
  EXPECT_EQ(testVec.x, 22500);
  EXPECT_EQ(testVec.y, 22500);
  EXPECT_EQ(testVec.z, 22500);
  EXPECT_EQ(testVec.w, 22500);

  testVec2 *= 6;
  EXPECT_EQ(testVec2.x, 1800);
  EXPECT_EQ(testVec2.y, 1800);
  EXPECT_EQ(testVec2.z, 1800);
  EXPECT_EQ(testVec2.w, 1800);

  testVec2 = testVec * 2;
  EXPECT_EQ(testVec2.x, 45000);
  EXPECT_EQ(testVec2.y, 45000);
  EXPECT_EQ(testVec2.z, 45000);
  EXPECT_EQ(testVec2.w, 45000);
}