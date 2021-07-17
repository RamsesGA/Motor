#define GTEST_HAS_TR1_TUPLE 0
#define GTEST_USE_OWN_TR1_TUPLE 0
#include <gtest/gtest.h>

#include <gaPrerequisitesUtilities.h>
#include <gaMatrix4x4.h>

using namespace gaEngineSDK;

TEST(gaUtilities, Matrix_4x4_Tests) {
  Matrix4x4 testMat4(1.0f,  5.0f, 9.0f,  13.0f,
                     2.0f,  6.0f, 10.0f, 14.0f, 
                     3.0f,  7.0f, 11.0f, 15.0f, 
                     4.0f,  8.0f, 12.0f, 16.0f);

  EXPECT_FLOAT_EQ(testMat4(0, 0), 1.0f);
  EXPECT_FLOAT_EQ(testMat4(1, 1), 6.0f);
  EXPECT_FLOAT_EQ(testMat4(2, 2), 11.0f);
  EXPECT_FLOAT_EQ(testMat4(3, 3), 16.0f);

  /*
  *   0     1       2      3    
  0- 1.0f,  2.0f,  3.0f,  4.0f, 
  1- 5.0f,  6.0f,  7.0f,  8.0f,
  2- 9.0f,  10.0f, 11.0f, 12.0f,
  3- 13.0f, 14.0f, 15.0f, 16.0f
  */
  testMat4 = testMat4.transpose();
  float accountant = 1.0f;
  for (int32 i = 0; i < testMat4.getRows(); ++i) {
    for (int32 j = 0; j < testMat4.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMat4(i, j), accountant);
      accountant++;
    }
  }

  /*
  *   0     1       2      3
  0- 1.0f, 5.0f, 9.0f,  13.0f, 
  1- 2.0f, 6.0f, 10.0f, 14.0f, 
  2- 3.0f, 7.0f, 11.0f, 15.0f, 
  3- 4.0f, 8.0f, 12.0f, 16.0f 
  */
  Matrix4x4 testMatrix2(testMat4);
  testMatrix2 = testMatrix2.transpose();
  accountant = 1.0f;
  for (int32 i = 0; i < testMatrix2.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix2.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix2(j, i), accountant);
      accountant++;
    }
  }

  /*
  *   0     1       2      3    
  0- 2.0f,  10.0f,  18.0f, 26.0f
  1- 4.0f,  12.0f,  20.0f, 28.0f
  2- 6.0f,  14.0f,  22.0f, 30.0f
  3- 8.0f,  16.0f,  24.0f, 32.0f
  */
  testMat4 = testMatrix2 + testMatrix2;
  accountant = 2.0f;
  for (int32 i = 0; i < testMat4.getRows(); ++i) {
    for (int32 j = 0; j < testMat4.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMat4(j, i), accountant);
      accountant += 2.0f;
    }
  }

  /*
  *   0     1       2      3  
  0- 1.0f, 5.0f, 9.0f,  13.0f,
  1- 2.0f, 6.0f, 10.0f, 14.0f,
  2- 3.0f, 7.0f, 11.0f, 15.0f,
  3- 4.0f, 8.0f, 12.0f, 16.0f 
  */
  testMat4 = testMat4 - testMatrix2;
  accountant = 1.0f;
  for (int32 i = 0; i < testMat4.getRows(); ++i) {
    for (int32 j = 0; j < testMat4.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMat4(j, i), accountant);
      accountant++;
    }
  }

  /*
  *   0        1        2        3    
  0- 90.0f,  202.0f, 314.0f,  426.0f,
  1- 100.0f, 228.0f, 356.0f,  484.0f,
  2- 110.0f, 254.0f, 398.0f,  542.0f,
  3- 120.0f, 280.0f, 440.0f,  600.0f 
  */
  testMatrix2 = testMat4 * testMatrix2;
  accountant = 90.0f;
  for (int32 j = 0; j < testMatrix2.getRows(); ++j) {
    EXPECT_FLOAT_EQ(testMatrix2(j, 0), accountant);
    accountant += 10.0f;
  }
  accountant = 202.0f;
  for (int32 j = 0; j < testMatrix2.getRows(); ++j) {
    EXPECT_FLOAT_EQ(testMatrix2(j, 1), accountant);
    accountant += 26.0f;
  }
  accountant = 314.0f;
  for (int32 j = 0; j < testMatrix2.getRows(); ++j) {
    EXPECT_FLOAT_EQ(testMatrix2(j, 2), accountant);
    accountant += 42.0f;
  }
  accountant = 426.0f;
  for (int32 j = 0; j < testMatrix2.getRows(); ++j) {
    EXPECT_FLOAT_EQ(testMatrix2(j, 3), accountant);
    accountant += 58.0f;
  }

  /*
  *    0     1        2      3   
  0- 51.0f, 56.0f,  51.0f, 56.0f,
  1- 52.0f, 57.0f,  52.0f, 57.0f,
  2- 53.0f, 58.0f,  53.0f, 58.0f,
  3- 54.0f, 59.0f,  54.0f, 59.0f
  */
  testMat4 = testMat4 + 50.0f;
  accountant = 51.0f;
  for (int32 i = 0; i < testMat4.getRows(); ++i) {
    for (int32 j = 0; j < testMat4.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMat4(j, i), accountant);
      accountant++;
    }
  }

  /*
  *   0     1       2      3   
  0- 1.0f, 6.0f,  11.0f, 16.0f,
  1- 2.0f, 7.0f,  12.0f, 17.0f,
  2- 3.0f, 8.0f,  13.0f, 18.0f,
  3- 4.0f, 9.0f,  14.0f, 19.0f
  */
  testMat4 = testMat4 - 50.0f;
  accountant = 1.0f;
  for (int32 i = 0; i < testMat4.getRows(); ++i) {
    for (int32 j = 0; j < testMat4.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMat4(j, i), accountant);
      accountant++;
    }
  }

  /*
  *   0       1       2      3   
  0- 5.0f,  30.0f,  55.0f, 80.0f,
  1- 10.0f, 35.0f,  60.0f, 85.0f,
  2- 15.0f, 40.0f,  65.0f, 90.0f,
  3- 20.0f, 45.0f,  70.0f, 95.0f
  */
  testMat4 = testMat4 * 5.0f;
  accountant = 5.0f;
  for (int32 i = 0; i < testMat4.getRows(); ++i) {
    for (int32 j = 0; j < testMat4.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMat4(j, i), accountant);
      accountant += 5.0f;
    }
  }

  /*
  *   0     1       2      3   
  0- 1.0f, 6.0f,  11.0f, 16.0f,
  1- 2.0f, 7.0f,  12.0f, 17.0f,
  2- 3.0f, 8.0f,  13.0f, 18.0f,
  3- 4.0f, 9.0f,  14.0f, 19.0f
  */
  testMat4 = testMat4 / 5.0f;
  accountant = 1.0f;
  for (int32 i = 0; i < testMat4.getRows(); ++i) {
    for (int32 j = 0; j < testMat4.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMat4(j, i), accountant);
      accountant++;
    }
  }

  //New 09/04/21

  Vector3 right;

  /*
  *   0     1       2      3
  0- 1.0f, 5.0f,  9.0f,  13.0f,
  1- 2.0f, 6.0f,  10.0f, 14.0f,
  2- 3.0f, 7.0f,  11.0f, 15.0f,
  3- 4.0f, 8.0f,  12.0f, 16.0f
  */
  right = testMat4.getColumnMatrixInfo(0);
  EXPECT_FLOAT_EQ(right.x, 1.0f);
  EXPECT_FLOAT_EQ(right.y, 5.0f);
  EXPECT_FLOAT_EQ(right.z, 9.0f);

  right = testMat4.getColumnMatrixInfo(1);
  EXPECT_FLOAT_EQ(right.x, 2.0f);
  EXPECT_FLOAT_EQ(right.y, 6.0f);
  EXPECT_FLOAT_EQ(right.z, 10.0f);

  right = testMat4.getColumnMatrixInfo(2);
  EXPECT_FLOAT_EQ(right.x, 3.0f);
  EXPECT_FLOAT_EQ(right.y, 7.0f);
  EXPECT_FLOAT_EQ(right.z, 11.0f);

  Matrix4x4 b;
  EXPECT_FLOAT_EQ(b.getColumnMatrixInfo(0).x, 0.0f);
  EXPECT_FLOAT_EQ(b.getColumnMatrixInfo(1).y, 0.0f);
  EXPECT_FLOAT_EQ(b.getColumnMatrixInfo(2).z, 0.0f);
}