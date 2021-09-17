#include "gaMatrix4x4.h"
#include "gaPlatformMath.h"

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * Constructor and Destructor
  */
  /***************************************************************************/
  
  Matrix4x4::Matrix4x4() {
    m_mat4x4[0][0] = 1.0f; m_mat4x4[0][1] = 0.0f;
    m_mat4x4[1][0] = 0.0f; m_mat4x4[1][1] = 1.0f;
    m_mat4x4[2][0] = 0.0f; m_mat4x4[2][1] = 0.0f;
    m_mat4x4[3][0] = 0.0f; m_mat4x4[3][1] = 0.0f;
  
    m_mat4x4[0][2] = 0.0f; m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][2] = 0.0f; m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][2] = 1.0f; m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][2] = 0.0f; m_mat4x4[3][3] = 1.0f;
  }
  
  Matrix4x4::Matrix4x4(float x1, float y1, float z1, float w1,
                       float x2, float y2, float z2, float w2,
                       float x3, float y3, float z3, float w3,
                       float x4, float y4, float z4, float w4) {
    m_mat4x4[0][0] = x1; m_mat4x4[0][1] = y1;
    m_mat4x4[1][0] = x2; m_mat4x4[1][1] = y2;
    m_mat4x4[2][0] = x3; m_mat4x4[2][1] = y3;
    m_mat4x4[3][0] = x4; m_mat4x4[3][1] = y4;
  
    m_mat4x4[0][2] = z1; m_mat4x4[0][3] = w1;
    m_mat4x4[1][2] = z2; m_mat4x4[1][3] = w2;
    m_mat4x4[2][2] = z3; m_mat4x4[2][3] = w3;
    m_mat4x4[3][2] = z4; m_mat4x4[3][3] = w4;
  }
  
  Matrix4x4::Matrix4x4(float x0, float y3, float z6,
                       float x1, float y4, float z7,
                       float x2, float y5, float z8) {
    m_mat4x4[0][0] = x0;   m_mat4x4[0][1] = y3;
    m_mat4x4[1][0] = x1;   m_mat4x4[1][1] = y4;
    m_mat4x4[2][0] = x2;   m_mat4x4[2][1] = y5;
    m_mat4x4[3][0] = 0.0f; m_mat4x4[3][1] = 0.0f;
  
    m_mat4x4[0][2] = z6;   m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][2] = z7;   m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][2] = z8;   m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][2] = 0.0f; m_mat4x4[3][3] = 0.0f;
  }
  
  Matrix4x4::Matrix4x4(const Matrix4x4& matrix) {
    for (int32 i = 0; i < matrix.getColumns(); ++i) {
      for (int32 j = 0; j < matrix.getRows(); ++j) {
        this->m_mat4x4[i][j] = matrix.m_mat4x4[i][j];
      }
    }
  }
  
  Matrix4x4::Matrix4x4(const Vector3& vecX,
                       const Vector3& vecY,
                       const Vector3& vecZ,
                       const Vector3& vecW) {
    m_mat4x4[0][0] = vecX.x; m_mat4x4[0][1] = vecX.y;
    m_mat4x4[1][0] = vecY.x; m_mat4x4[1][1] = vecY.y;
    m_mat4x4[2][0] = vecZ.x; m_mat4x4[2][1] = vecZ.y;
    m_mat4x4[3][0] = vecW.x; m_mat4x4[3][1] = vecW.y;
  
    m_mat4x4[0][2] = vecX.z; m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][2] = vecY.z; m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][2] = vecZ.z; m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][2] = vecW.z; m_mat4x4[3][3] = 1.0f;
  }
  
  /***************************************************************************/
  /**
  * Normal methods.
  */
  /***************************************************************************/
  
  Matrix4x4&
  Matrix4x4::transpose() {
    Matrix4x4 trans;
  
    for (int32 i = 0; i < 4; ++i) {
      for (int32 j = 0; j < 4; ++j) {
        trans(i, j) = this->m_mat4x4[j][i];
      }
    }
    *this = trans;
    return *this;
  }
  
  Matrix4x4
  Matrix4x4::perspectiveFovLH(float FOV, float width, float height, float near, float farF) {
    float rad = FOV;
    float h = Math::cos(float(0.5) * rad) / Math::sin(float(0.5) * rad);
    auto w = h * height / width;
  
    Matrix4x4 result(w, 0, 0,                                  0,
                     0, h, 0,                                  0,
                     0, 0, (farF + near) / (farF - near),      1,
                     0, 0, -(2 * farF * near) / (farF - near), 0);
    return result;
  }
  
  Matrix4x4 
  Matrix4x4::matrixLookAtLH(const Vector3* pEye, const Vector3* pAt, const Vector3* pUp) {
    //to left hand
    Matrix4x4 view;
    Vector3 cameraFront = pAt - pEye;
    cameraFront.normalize();

    Vector3 cameraRight = pUp->crossProduct(cameraFront);
    cameraRight.normalize();

    Vector3 realUp = cameraFront.crossProduct(cameraRight);

    Vector3 up = realUp;

    view = {
             cameraRight.x, cameraRight.y, cameraRight.z, 0.0f,
             up.x,          up.y,          up.z,          0.0f,
             cameraFront.x, cameraFront.y, cameraFront.z, 0.0f,
             0.0f,          0.0f,          0.0f,          1.0f
           };

    Matrix4x4 pos = {
                      1.0f, 0.0f, 0.0f, -pEye->x,
                      0.0f, 1.0f, 0.0f, -pEye->y,
                      0.0f, 0.0f, 1.0f, -pEye->z,
                      0.0f, 0.0f, 0.0f, 1.0f
                    };

    view *= pos;
    view.transpose();

    return view;
  }

  Vector3
  Matrix4x4::getColumnMatrixInfo(uint32 index) {
    Vector<Vector3>matrixData;
    matrixData.resize(3);
  
    matrixData.at(0).x = m_mat4x4[0][0];
    matrixData.at(0).y = m_mat4x4[1][0];
    matrixData.at(0).z = m_mat4x4[2][0];
  
    matrixData.at(1).x = m_mat4x4[0][1];
    matrixData.at(1).y = m_mat4x4[1][1];
    matrixData.at(1).z = m_mat4x4[2][1];
  
    matrixData.at(2).x = m_mat4x4[0][2];
    matrixData.at(2).y = m_mat4x4[1][2];
    matrixData.at(2).z = m_mat4x4[2][2];
    return matrixData.at(index);
  }
  
  void
  Matrix4x4::rotationX(float speedRotation) {
    m_mat4x4[0][0] = 1.0f;
    m_mat4x4[1][0] = 0.0f;
    m_mat4x4[2][0] = 0.0f;
    m_mat4x4[3][0] = 0.0f;
  
    m_mat4x4[0][1] = 0.0f;
    m_mat4x4[1][1] = cosf(speedRotation);
    m_mat4x4[2][1] = -sinf(speedRotation);
    m_mat4x4[3][1] = 0.0f;
  
    m_mat4x4[0][2] = 0.0f;
    m_mat4x4[1][2] = sinf(speedRotation);
    m_mat4x4[2][2] = cosf(speedRotation);
    m_mat4x4[3][2] = 0.0f;
  
    m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][3] = 1.0f;
  }
  
  Matrix4x4&
  Matrix4x4::rotationX(const Radians& angleRadian) {
    Matrix4x4 rotX =
    {
     1.0f, 0.0f, 0.0f, 0.0f,
     0.0f, cosf(angleRadian.valueRadians()),-sinf(angleRadian.valueRadians()), 0,
     0.0f, sinf(angleRadian.valueRadians()), cosf(angleRadian.valueRadians()), 0,
     0.0f, 0.0f, 0.0f, 1.0f
    };
  
    *this = rotX * (*this);
    return *this;
  }
  
  void
  Matrix4x4::rotationY(float speedRotation) {
    m_mat4x4[0][0] = cosf(speedRotation);
    m_mat4x4[1][0] = 0.0f;
    m_mat4x4[2][0] = sinf(speedRotation);
    m_mat4x4[3][0] = 0.0f;
  
    m_mat4x4[0][1] = 0.0f;
    m_mat4x4[1][1] = 1.0f;
    m_mat4x4[2][1] = 0.0f;
    m_mat4x4[3][1] = 0.0f;
  
    m_mat4x4[0][2] = -sinf(speedRotation);
    m_mat4x4[1][2] = 0.0f;
    m_mat4x4[2][2] = cosf(speedRotation);
    m_mat4x4[3][2] = 0.0f;
  
    m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][3] = 1.0f;
  }
  
  Matrix4x4&
  Matrix4x4::rotationY(const Radians& angleRadian) {
    Matrix4x4 rotY =
    {
     Math::cos(angleRadian.valueRadians()), 0, Math::sin(angleRadian.valueRadians()), 0,
               0.0f,                     1.0f, 0.0f,                               0.0f,
     -Math::sin(angleRadian.valueRadians()), 0, Math::cos(angleRadian.valueRadians()), 0,
               0.0f,                      0.0f, 0.0f,                               1.0f
    };
  
    *this = rotY * (*this);
    return *this;
  }
  
  void
  Matrix4x4::rotationZ(float speedRotation) {
    m_mat4x4[0][0] = cosf(speedRotation);
    m_mat4x4[1][0] = -sinf(speedRotation);
    m_mat4x4[2][0] = 0.0f;
    m_mat4x4[3][0] = 0.0f;
  
    m_mat4x4[0][1] = sinf(speedRotation);
    m_mat4x4[1][1] = cosf(speedRotation);
    m_mat4x4[2][1] = 0.0f;
    m_mat4x4[3][1] = 0.0f;
  
    m_mat4x4[0][2] = 0.0f;
    m_mat4x4[1][2] = 0.0f;
    m_mat4x4[2][2] = 1.0f;
    m_mat4x4[3][2] = 0.0f;

    m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][3] = 1.0f;
  }
  
  void
  Matrix4x4::calculateAxis(Vector3 right, Vector3 up, Vector3 front) {
    m_mat4x4[0][0] = right.x; m_mat4x4[0][1] = up.x;
    m_mat4x4[1][0] = right.y; m_mat4x4[1][1] = up.y;
    m_mat4x4[2][0] = right.z; m_mat4x4[2][1] = up.z;
    m_mat4x4[3][0] = 0.0f;    m_mat4x4[3][1] = 0.0f;
  
    m_mat4x4[0][2] = front.x; m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][2] = front.y; m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][2] = front.z; m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][2] = 0.0f;    m_mat4x4[3][3] = 1.0f;
  }
  
  void
  Matrix4x4::calculatePosition(Vector3 eye) {
    m_mat4x4[0][0] = 1.0f;   m_mat4x4[0][1] = 0.0f;
    m_mat4x4[1][0] = 0.0f;   m_mat4x4[1][1] = 1.0f;
    m_mat4x4[2][0] = 0.0f;   m_mat4x4[2][1] = 0.0f;
    m_mat4x4[3][0] = -eye.x; m_mat4x4[3][1] = -eye.y;
  
    m_mat4x4[0][2] = 0.0f;   m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][2] = 0.0f;   m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][2] = 1.0f;   m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][2] = -eye.z; m_mat4x4[3][3] = 1.0f;
  }
  
  Matrix4x4&
  Matrix4x4::identity() {
    m_mat4x4[0][0] = 1.0f; m_mat4x4[0][1] = 0.0f;
    m_mat4x4[1][0] = 0.0f; m_mat4x4[1][1] = 1.0f;
    m_mat4x4[2][0] = 0.0f; m_mat4x4[2][1] = 0.0f;
    m_mat4x4[3][0] = 0.0f; m_mat4x4[3][1] = 0.0f;
  
    m_mat4x4[0][2] = 0.0f; m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][2] = 0.0f; m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][2] = 1.0f; m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][2] = 0.0f; m_mat4x4[3][3] = 1.0f;
    return *this;
  }
  
  Matrix4x4&
  Matrix4x4::scale(Vector3 scal) {
    scale(scal.x, scal.y, scal.z);
    return *this;
  }
  
  Matrix4x4&
  Matrix4x4::scale(float sx, float sy, float sz) {
    Matrix4x4 scal =
    {
      sx, 0, 0, 0,
      0, sy, 0, 0,
      0, 0, sz, 0,
      0, 0, 0, 1
    };
  
    *this *= scal;
    return *this;
  }
  
  Matrix4x4&
  Matrix4x4::translate(const Vector3& vec3) {
    translate(vec3.x, vec3.y, vec3.z);
    return *this;
  }
  
  Matrix4x4&
  Matrix4x4::translate(float X, float Y, float Z) {
    Matrix4x4 tras =
    {
      1, 0, 0, X,
      0, 1, 0, Y,
      0, 0, 1, Z,
      0, 0, 0, 1
    };
  
    *this *= tras;
    return *this;
  }
  
  Matrix4x4&
  Matrix4x4::invert(Matrix4x4 matrix) {
    float tempParamMatrix[4][4];
    float inverseMatrix[4][4];
  
    tempParamMatrix[0][0] = matrix.m_mat4x4[0][0];
    tempParamMatrix[1][0] = matrix.m_mat4x4[1][0];
    tempParamMatrix[2][0] = matrix.m_mat4x4[2][0];
    tempParamMatrix[3][0] = matrix.m_mat4x4[3][0];
  
    tempParamMatrix[0][1] = matrix.m_mat4x4[0][1];
    tempParamMatrix[1][1] = matrix.m_mat4x4[1][1];
    tempParamMatrix[2][1] = matrix.m_mat4x4[2][1];
    tempParamMatrix[3][1] = matrix.m_mat4x4[3][1];
  
    tempParamMatrix[0][2] = matrix.m_mat4x4[0][2];
    tempParamMatrix[1][2] = matrix.m_mat4x4[1][2];
    tempParamMatrix[2][2] = matrix.m_mat4x4[2][2];
    tempParamMatrix[3][2] = matrix.m_mat4x4[3][2];
  
    tempParamMatrix[0][3] = matrix.m_mat4x4[0][3];
    tempParamMatrix[1][3] = matrix.m_mat4x4[1][3];
    tempParamMatrix[2][3] = matrix.m_mat4x4[2][3];
    tempParamMatrix[3][3] = matrix.m_mat4x4[3][3];
  
    float det = getDeterminant(tempParamMatrix);
  
    if (0.0f == det) {
      return matrix;
    }
  
    // Find adjoint
    float adj[4][4];
    adjoint(tempParamMatrix, adj);
  
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (uint32 i = 0; i < 4; ++i) {
      for (uint32 j = 0; j < 4; ++j) {
        inverseMatrix[i][j] = adj[i][j] / float(det);
      }
    }
  
    matrix.m_mat4x4[0][0] = inverseMatrix[0][0];
    matrix.m_mat4x4[1][0] = inverseMatrix[1][0];
    matrix.m_mat4x4[2][0] = inverseMatrix[2][0];
    matrix.m_mat4x4[3][0] = inverseMatrix[3][0];
  
    matrix.m_mat4x4[0][1] = inverseMatrix[0][1];
    matrix.m_mat4x4[1][1] = inverseMatrix[1][1];
    matrix.m_mat4x4[2][1] = inverseMatrix[2][1];
    matrix.m_mat4x4[3][1] = inverseMatrix[3][1];
  
    matrix.m_mat4x4[0][2] = inverseMatrix[0][2];
    matrix.m_mat4x4[1][2] = inverseMatrix[1][2];
    matrix.m_mat4x4[2][2] = inverseMatrix[2][2];
    matrix.m_mat4x4[3][2] = inverseMatrix[3][2];
  
    matrix.m_mat4x4[0][3] = inverseMatrix[0][3];
    matrix.m_mat4x4[1][3] = inverseMatrix[1][3];
    matrix.m_mat4x4[2][3] = inverseMatrix[2][3];
    matrix.m_mat4x4[3][3] = inverseMatrix[3][3];
    return matrix;
  }
  
  void
  Matrix4x4::adjoint(float tempMatrix[4][4], float adj[4][4]) {
    // temp is used to store cofactors of A[][]
    int32 sign = 1;
    float temp[4][4];
  
    for (uint32 i = 0; i < 4; ++i) {
      for (uint32 j = 0; j < 4; ++j) {
        // Get cofactor of A[i][j]
        getCofactor(tempMatrix, temp, i, j, 4);
  
        // sign of adj[j][i] positive if sum of row
        // and column indexes is even.
        sign = ((i + j) % 2 == 0) ? 1 : -1;
  
        // Interchanging rows and columns to get the
        // transpose of the cofactor matrix
        adj[j][i] = (sign) * (getDeterminant(temp, 4 - 1));
      }
    }
  }
  
  /***************************************************************************/
  /**
  * Operators overloads.
  */
  /***************************************************************************/
  
  float&
  Matrix4x4::operator()(const uint32& row, const uint32& column) {
    return this->m_mat4x4[row][column];
  }
  
  Matrix4x4
  Matrix4x4::operator+(Matrix4x4& matrix)const {
    Matrix4x4 add;
  
    for (uint32 i = 0; i < 4; ++i) {
      for (uint32 j = 0; j < 4; ++j) {
        add(i, j) = this->m_mat4x4[i][j] + matrix(i, j);
      }
    }
    return add;
  }
  
  void
  Matrix4x4::operator+=(Matrix4x4& matrix) {
    *this = *this + matrix;
  }
  
  Matrix4x4
  Matrix4x4::operator-(Matrix4x4& matrix)const {
    Matrix4x4 subs;
  
    for (uint32 i = 0; i < 4; ++i) {
      for (uint32 j = 0; j < 4; ++j) {
        subs(i, j) = this->m_mat4x4[i][j] - matrix(i, j);
      }
    }
    return subs;
  }
  
  Matrix4x4
  Matrix4x4::operator*(Matrix4x4& matrix)const {
    Matrix4x4 multip;

    float temp = 0.0f;
  
    for (uint32 i = 0; i < 4; ++i) {
      for (uint32 j = 0; j < 4; ++j) {
        temp = 0.0f;
  
        for (uint32 k = 0; k < 4; ++k) {
          temp += m_mat4x4[i][k] * matrix(k, j);
        }
  
        multip(i, j) = temp;
      }
    }
    return multip;
  }
  
  Matrix4x4
  Matrix4x4::operator+(float data)const {
    Matrix4x4 result;
  
    for (uint32 i = 0; i < 4; ++i) {
      for (uint32 j = 0; j < 4; ++j) {
        result(i, j) = this->m_mat4x4[i][j] + data;
      }
    }
    return result;
  }
  
  Matrix4x4
  Matrix4x4::operator-(float data)const {
    Matrix4x4 result;
  
    for (uint32 i = 0; i < 4; ++i) {
      for (uint32 j = 0; j < 4; ++j) {
        result(i, j) = this->m_mat4x4[i][j] - data;
      }
    }
    return result;
  }
  
  Matrix4x4
  Matrix4x4::operator*(float data)const {
    Matrix4x4 result;
  
    for (uint32 i = 0; i < 4; ++i) {
      for (uint32 j = 0; j < 4; ++j) {
        result(i, j) = this->m_mat4x4[i][j] * data;
      }
    }
    return result;
  }
  
  Matrix4x4&
  Matrix4x4::operator*=(Matrix4x4& mat) {
    *this = *this * mat;
    return *this;
  }
  
  void
  Matrix4x4::operator*=(float data) {
    *this = *this * data;
  }
  
  Matrix4x4
  Matrix4x4::operator/(float data)const {
    Matrix4x4 result;
  
    for (uint32 i = 0; i < 4; ++i) {
      for (uint32 j = 0; j < 4; ++j) {
        result(i, j) = this->m_mat4x4[i][j] / data;
      }
    }
    return result;
  }
  
  /***************************************************************************/
  /**
  * Gets.
  */
  /***************************************************************************/
  
  int32
  Matrix4x4::getRows() const {
    return 4;
  }
  
  int32
  Matrix4x4::getColumns() const {
    return 4;
  }
  
  float
  Matrix4x4::getDeterminant(float tempMatrix[4][4], float n) {
    // Initialize result
    float D = 0.0f;
  
    //  Base case : if matrix contains single element
    if (n == 1) {
      return tempMatrix[0][0];
    }
  
    // To store cofactors
    float temp[4][4];
  
    // To store sign multiplier
    float sign = 1.0f;
  
    // Iterate for each element of first row
    for (uint32 f = 0; f < n; ++f) {
      // Getting Cofactor of A[0][f]
      getCofactor(tempMatrix, temp, 0, f, n);
      D += sign * tempMatrix[0][f] * getDeterminant(temp, n - 1.0f);
  
      // terms are to be added with alternate sign
      sign = -sign;
    }
    return D;
  }
  
  void
  Matrix4x4::getCofactor(float tempMatrix[4][4], 
                         float tempMatrix2[4][4], 
                         uint32 p, 
                         uint32 q, 
                         uint32 n) {
    uint32 i = 0, j = 0;
  
    // Looping for each element of the matrix
    for (uint32 row = 0; row < n; ++row) {
      for (uint32 col = 0; col < n; ++col) {
        //  Copying into temporary matrix only those element
        //  which are not in given row and column
        if (row != p && col != q) {
          tempMatrix2[i][j++] = tempMatrix[row][col];
  
          // Row is filled, so increase row index and
          // reset col index
          if (j == n - 1) {
            j = 0;
            i++;
          }
        }
      }
    }
  }
}