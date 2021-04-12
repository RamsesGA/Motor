#define GTEST_HAS_TR1_TUPLE 0
#define GTEST_USE_OWN_TR1_TUPLE 0
#include <gtest/gtest.h>

#include <gaPrerequisitesUtilities.h>
#include <gaMatrix.h>
#include <gaMatrix4x4.h>

using namespace gaEngineSDK;

TEST(gaUtilities, Matrix_NxN_Tests) {
  Matrix testMatrix(5, 5);
  EXPECT_EQ(testMatrix.getColumns(), 5);
  EXPECT_EQ(testMatrix.getRows(), 5);

  /*
  *   0    1    2    3    4
  0- 1.0f,0.0f,0.0f,0.0f,0.0f,
  1- 0.0f,1.0f,0.0f,0.0f,0.0f,
  2- 0.0f,0.0f,1.0f,0.0f,0.0f,
  3- 0.0f,0.0f,0.0f,1.0f,0.0f,
  4- 0.0f,0.0f,0.0f,0.0f,1.0f
  */

  //0-
  EXPECT_FLOAT_EQ(testMatrix(0, 0), 1.0f);
  EXPECT_FLOAT_EQ(testMatrix(1, 0), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(2, 0), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(3, 0), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(4, 0), 0.0f);
  //1-
  EXPECT_FLOAT_EQ(testMatrix(0, 1), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(1, 1), 1.0f);
  EXPECT_FLOAT_EQ(testMatrix(2, 1), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(3, 1), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(4, 1), 0.0f);
  //2-
  EXPECT_FLOAT_EQ(testMatrix(0, 2), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(1, 2), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(2, 2), 1.0f);
  EXPECT_FLOAT_EQ(testMatrix(3, 2), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(4, 2), 0.0f);
  //3-                          
  EXPECT_FLOAT_EQ(testMatrix(0, 3), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(1, 3), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(2, 3), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(3, 3), 1.0f);
  EXPECT_FLOAT_EQ(testMatrix(4, 3), 0.0f);
  //4-                           
  EXPECT_FLOAT_EQ(testMatrix(0, 4), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(1, 4), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(2, 4), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(3, 4), 0.0f);
  EXPECT_FLOAT_EQ(testMatrix(4, 4), 1.0f);
}

TEST(gaUtilities, Matrix_Tests) {
  Matrix testMatrix(5, 5);

  float accountant = 1.0f;
  for (int32 i = 0; i < testMatrix.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix.getColumns(); ++j) {
      testMatrix(i, j) = accountant;
      accountant++;
    }
  }

  /*
  *   0     1       2      3      4
  0- 1.0f, 6.0f,  11.0f, 16.0f, 21.0f,
  1- 2.0f, 7.0f,  12.0f, 17.0f, 22.0f,
  2- 3.0f, 8.0f,  13.0f, 18.0f, 23.0f,
  3- 4.0f, 9.0f,  14.0f, 19.0f, 24.0f,
  4- 5.0f, 10.0f, 15.0f, 20.0f, 25.0f
  */

  //O de manera automática
  accountant = 1.0f;
  for (int32 i = 0; i < testMatrix.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix(i, j), accountant);
      accountant++;
    }
  }

  /*
  *   0     1       2      3      4
  0- 1.0f,  2.0f,  3.0f,  4.0f,  5.0f,
  1- 6.0f,  7.0f,  8.0f,  9.0f,  10.0f,
  2- 11.0f, 12.0f, 13.0f, 14.0f, 15.0f,
  3- 16.0f, 17.0f, 18.0f, 19.0f, 20.0f,
  4- 21.0f, 22.0f, 23.0f, 24.0f, 25.0f
  */
  testMatrix = testMatrix.transpose();
  accountant = 1.0f;
  for (int32 i = 0; i < testMatrix.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix(j, i), accountant);
      accountant++;
    }
  }

  /*
  *   0     1       2      3      4
  0- 1.0f, 6.0f,  11.0f, 16.0f, 21.0f,
  1- 2.0f, 7.0f,  12.0f, 17.0f, 22.0f,
  2- 3.0f, 8.0f,  13.0f, 18.0f, 23.0f,
  3- 4.0f, 9.0f,  14.0f, 19.0f, 24.0f,
  4- 5.0f, 10.0f, 15.0f, 20.0f, 25.0f
  */
  Matrix testMatrix2(testMatrix);
  testMatrix2 = testMatrix2.transpose();
  accountant = 1.0f;
  for (int32 i = 0; i < testMatrix2.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix2.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix2(i, j), accountant);
      accountant++;
    }
  }

  /*
  *   0     1       2      3      4
  0- 2.0f,  12.0f,  22.0f, 32.0f, 42.0f,
  1- 4.0f,  14.0f,  24.0f, 34.0f, 44.0f,
  2- 6.0f,  16.0f,  26.0f, 36.0f, 46.0f,
  3- 8.0f,  18.0f,  28.0f, 38.0f, 48.0f,
  4- 10.0f, 20.0f,  30.0f, 40.0f, 50.0f
  */
  testMatrix = testMatrix2 + testMatrix2;
  accountant = 2.0f;
  for (int32 i = 0; i < testMatrix.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix(i, j), accountant);
      accountant += 2.0f;
    }
  }

  /*
  *   0     1       2      3      4
  0- 1.0f, 6.0f,  11.0f, 16.0f, 21.0f,
  1- 2.0f, 7.0f,  12.0f, 17.0f, 22.0f,
  2- 3.0f, 8.0f,  13.0f, 18.0f, 23.0f,
  3- 4.0f, 9.0f,  14.0f, 19.0f, 24.0f,
  4- 5.0f, 10.0f, 15.0f, 20.0f, 25.0f
  */
  testMatrix = testMatrix - testMatrix2;
  accountant = 1.0f;
  for (int32 i = 0; i < testMatrix.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix(i, j), accountant);
      accountant++;
    }
  }

  /*
  *   0        1        2        3        4
  0- 215.0f, 490.0f, 765.0f,  1040.0f, 1315.0f,
  1- 230.0f, 530.0f, 830.0f,  1130.0f, 1430.0f,
  2- 245.0f, 570.0f, 895.0f,  1220.0f, 1545.0f,
  3- 260.0f, 610.0f, 960.0f,  1310.0f, 1660.0f,
  4- 275.0f, 650.0f, 1025.0f, 1400.0f, 1775.0f
  */
  testMatrix2 = testMatrix * testMatrix2;
  accountant = 215.0f;
  for (int32 j = 0; j < testMatrix2.getColumns(); ++j) {
    EXPECT_FLOAT_EQ(testMatrix2(0, j), accountant);
    accountant += 15.0f;
  }
  accountant = 490.0f;
  for (int32 j = 0; j < testMatrix2.getColumns(); ++j) {
    EXPECT_FLOAT_EQ(testMatrix2(1, j), accountant);
    accountant += 40.0f;
  }
  accountant = 765.0f;
  for (int32 j = 0; j < testMatrix2.getColumns(); ++j) {
    EXPECT_FLOAT_EQ(testMatrix2(2, j), accountant);
    accountant += 65.0f;
  }
  accountant = 1040.0f;
  for (int32 j = 0; j < testMatrix2.getColumns(); ++j) {
    EXPECT_FLOAT_EQ(testMatrix2(3, j), accountant);
    accountant += 90.0f;
  }
  accountant = 1315.0f;
  for (int32 j = 0; j < testMatrix2.getColumns(); ++j) {
    EXPECT_FLOAT_EQ(testMatrix2(4, j), accountant);
    accountant += 115.0f;
  }

  /*
  *    0     1        2      3      4
  0- 51.0f, 56.0f,  51.0f, 56.0f, 71.0f,
  1- 52.0f, 57.0f,  52.0f, 57.0f, 72.0f,
  2- 53.0f, 58.0f,  53.0f, 58.0f, 73.0f,
  3- 54.0f, 59.0f,  54.0f, 59.0f, 74.0f,
  4- 55.0f, 60.0f,  65.0f, 70.0f, 75.0f
  */
  testMatrix = testMatrix + 50.0f;
  accountant = 51.0f;
  for (int32 i = 0; i < testMatrix.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix(i, j), accountant);
      accountant++;
    }
  }

  /*
  *   0     1       2      3      4
  0- 1.0f, 6.0f,  11.0f, 16.0f, 21.0f,
  1- 2.0f, 7.0f,  12.0f, 17.0f, 22.0f,
  2- 3.0f, 8.0f,  13.0f, 18.0f, 23.0f,
  3- 4.0f, 9.0f,  14.0f, 19.0f, 24.0f,
  4- 5.0f, 10.0f, 15.0f, 20.0f, 25.0f
  */
  testMatrix = testMatrix - 50.0f;
  accountant = 1.0f;
  for (int32 i = 0; i < testMatrix.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix(i, j), accountant);
      accountant++;
    }
  }

  /*
  *   0       1       2      3      4
  0- 5.0f,  30.0f,  55.0f, 80.0f,  105.0f,
  1- 10.0f, 35.0f,  60.0f, 85.0f,  110.0f,
  2- 15.0f, 40.0f,  65.0f, 90.0f,  115.0f,
  3- 20.0f, 45.0f,  70.0f, 95.0f,  120.0f,
  4- 25.0f, 50.0f,  75.0f, 100.0f, 125.0f
  */
  testMatrix = testMatrix * 5.0f;
  accountant = 5.0f;
  for (int32 i = 0; i < testMatrix.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix(i, j), accountant);
      accountant += 5.0f;
    }
  }

  /*
  *   0     1       2      3      4
  0- 1.0f, 6.0f,  11.0f, 16.0f, 21.0f,
  1- 2.0f, 7.0f,  12.0f, 17.0f, 22.0f,
  2- 3.0f, 8.0f,  13.0f, 18.0f, 23.0f,
  3- 4.0f, 9.0f,  14.0f, 19.0f, 24.0f,
  4- 5.0f, 10.0f, 15.0f, 20.0f, 25.0f
  */
  testMatrix = testMatrix / 5.0f;
  accountant = 1.0f;
  for (int32 i = 0; i < testMatrix.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix(i, j), accountant);
      accountant++;
    }
  }

  EXPECT_FLOAT_EQ(testMatrix(1, 1), 7.0f);
  EXPECT_FLOAT_EQ(testMatrix2(1, 1), 530.0f);
}

TEST(gaUtilities, Matrix_2x2_Tests) {
  Vector2 testVec(5.0f, 10.0f);
  Vector2 testVec2(15.0f, 20.0f);
  Matrix testMatrix2(testVec, testVec2);
  EXPECT_FLOAT_EQ(testMatrix2(0, 0), 5.0f);
  EXPECT_FLOAT_EQ(testMatrix2(1, 1), 20.0f);

  /*
  *   0      1     
  0- 5.0f,  20.0f, 
  1- 10.0f, 25.0f
  */
  testMatrix2 = testMatrix2.transpose();
  float accountant = 5.0f;
  for (int32 i = 0; i < testMatrix2.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix2.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix2(j, i), accountant);
      accountant += 5.0f;
    }
  }
}

TEST(gaUtilities, Matrix_3x3_Tests) {
  Vector3 testVec(5.0f, 10.0f, 15.0f);
  Vector3 testVec2(20.0f, 25.0f, 30.0f);
  Vector3 testVec3(35.0f, 40.0f, 45.0f);
  Matrix testMatrix3(testVec, testVec2, testVec3);
  EXPECT_FLOAT_EQ(testMatrix3(0, 0), 5.0f);
  EXPECT_FLOAT_EQ(testMatrix3(1, 1), 25.0f);
  EXPECT_FLOAT_EQ(testMatrix3(2, 2), 45.0f);

  /*
  *   0      1     2    
  0- 5.0f,  20.0f, 35.0f, 
  1- 10.0f, 25.0f, 40.0f,
  2- 15.0f, 30.0f, 45.0f
  */
  testMatrix3 = testMatrix3.transpose();
  float accountant = 5.0f;
  for (int32 i = 0; i < testMatrix3.getRows(); ++i) {
    for (int32 j = 0; j < testMatrix3.getColumns(); ++j) {
      EXPECT_FLOAT_EQ(testMatrix3(j, i), accountant);
      accountant += 5.0f;
    }
  }
}

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
  right = testMat4.matrixData3(0);
  EXPECT_FLOAT_EQ(right.m_x, 1.0f);
  EXPECT_FLOAT_EQ(right.m_y, 5.0f);
  EXPECT_FLOAT_EQ(right.m_z, 9.0f);

  right = testMat4.matrixData3(1);
  EXPECT_FLOAT_EQ(right.m_x, 2.0f);
  EXPECT_FLOAT_EQ(right.m_y, 6.0f);
  EXPECT_FLOAT_EQ(right.m_z, 10.0f);

  right = testMat4.matrixData3(2);
  EXPECT_FLOAT_EQ(right.m_x, 3.0f);
  EXPECT_FLOAT_EQ(right.m_y, 7.0f);
  EXPECT_FLOAT_EQ(right.m_z, 11.0f);

  Matrix4x4 b;
  EXPECT_FLOAT_EQ(b.matrixData3(0).m_x, 0.0f);
  EXPECT_FLOAT_EQ(b.matrixData3(1).m_y, 0.0f);
  EXPECT_FLOAT_EQ(b.matrixData3(2).m_z, 0.0f);
}