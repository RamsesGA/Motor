#include "gaMatrix4x4.h"
#include "gaPlatformMath.h"

namespace gaEngineSDK {

  /*************************************************************************/
  /**
  * Constructor and Destructor
  */
  /*************************************************************************/

  Matrix4x4::Matrix4x4(int32 columnSize, int32 rowSize) {
    m_columnSize = columnSize;
    m_rowSize = rowSize;

    m_mat4x4[0][0] = 0.0f; m_mat4x4[0][1] = 0.0f;
    m_mat4x4[1][0] = 0.0f; m_mat4x4[1][1] = 0.0f;
    m_mat4x4[2][0] = 0.0f; m_mat4x4[2][1] = 0.0f;
    m_mat4x4[3][0] = 0.0f; m_mat4x4[3][1] = 0.0f;

    m_mat4x4[0][2] = 0.0f; m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][2] = 0.0f; m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][2] = 0.0f; m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][2] = 0.0f; m_mat4x4[3][3] = 0.0f;
  }

  Matrix4x4::Matrix4x4() {
    m_columnSize = 4;
    m_rowSize = 4;

    m_mat4x4[0][0] = 0.0f; m_mat4x4[0][1] = 0.0f;
    m_mat4x4[1][0] = 0.0f; m_mat4x4[1][1] = 0.0f;
    m_mat4x4[2][0] = 0.0f; m_mat4x4[2][1] = 0.0f;
    m_mat4x4[3][0] = 0.0f; m_mat4x4[3][1] = 0.0f;

    m_mat4x4[0][2] = 0.0f; m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][2] = 0.0f; m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][2] = 0.0f; m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][2] = 0.0f; m_mat4x4[3][3] = 0.0f;
  }

  Matrix4x4::Matrix4x4(float x1, float y1, float z1, float w1,
                       float x2, float y2, float z2, float w2,
                       float x3, float y3, float z3, float w3,
                       float x4, float y4, float z4, float w4) {
    m_columnSize = 4;
    m_rowSize = 4;
    m_mat4x4[0][0] = x1; m_mat4x4[0][1] = y1;
    m_mat4x4[1][0] = x2; m_mat4x4[1][1] = y2;
    m_mat4x4[2][0] = x3; m_mat4x4[2][1] = y3;
    m_mat4x4[3][0] = x4; m_mat4x4[3][1] = y4;

    m_mat4x4[0][2] = z1; m_mat4x4[0][3] = w1;
    m_mat4x4[1][2] = z2; m_mat4x4[1][3] = w2;
    m_mat4x4[2][2] = z3; m_mat4x4[2][3] = w3;
    m_mat4x4[3][2] = z4; m_mat4x4[3][3] = w4;
  }

  Matrix4x4::Matrix4x4(const Matrix4x4& matrix){
    this->m_columnSize = matrix.getColumns();
    this->m_rowSize = matrix.getRows();

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
    m_columnSize = 4;
    m_rowSize = 4;
    m_mat4x4[0][0] = vecX.m_x; m_mat4x4[0][1] = vecX.m_y;
    m_mat4x4[1][0] = vecY.m_x; m_mat4x4[1][1] = vecY.m_y;
    m_mat4x4[2][0] = vecZ.m_x; m_mat4x4[2][1] = vecZ.m_y;
    m_mat4x4[3][0] = vecW.m_x; m_mat4x4[3][1] = vecW.m_y;

    m_mat4x4[0][2] = vecX.m_z; m_mat4x4[0][3] = 0.0f;
    m_mat4x4[1][2] = vecY.m_z; m_mat4x4[1][3] = 0.0f;
    m_mat4x4[2][2] = vecZ.m_z; m_mat4x4[2][3] = 0.0f;
    m_mat4x4[3][2] = vecW.m_z; m_mat4x4[3][3] = 1.0f;
  }

  /*************************************************************************/
  /**
  * Normal methods.
  */
  /*************************************************************************/

  Matrix4x4 
  Matrix4x4::transpose() {
    Matrix4x4 trans(4, 4);

    for (int32 i = 0; i < 4; ++i) {
      for (int32 j = 0; j < 4; ++j) {
        trans(i, j) = this->m_mat4x4[j][i];
      }
    }

    return trans;
  }

  Matrix4x4 
  Matrix4x4::perspectiveFovLH(float FOV, float width,
                              float height, float near,
                              float far) {
    float rad = FOV;
    float h = Math::cos(float(0.5) * rad) / Math::sin(float(0.5) * rad);
    auto w = h * height / width;

    Matrix4x4 result(w,   0,   0,                                0,
                     0,   h,   0,                                0,
                     0,   0,   (far + near) / (far - near),      1,
                     0,   0,   -(2 * far * near) / (far - near), 0);
    return result;
  }

  Vector3 
  Matrix4x4::matrixData3(uint32 index) {
    m_matrixData.resize(3);
    m_matrixData.at(0).m_x = m_mat4x4[0][0];
    m_matrixData.at(0).m_y = m_mat4x4[0][1];
    m_matrixData.at(0).m_z = m_mat4x4[0][2];
                           
    m_matrixData.at(1).m_x = m_mat4x4[1][0];
    m_matrixData.at(1).m_y = m_mat4x4[1][1];
    m_matrixData.at(1).m_z = m_mat4x4[1][2];
                           
    m_matrixData.at(2).m_x = m_mat4x4[2][0];
    m_matrixData.at(2).m_y = m_mat4x4[2][1];
    m_matrixData.at(2).m_z = m_mat4x4[2][2];

    return m_matrixData.at(index);
  }

  /*************************************************************************/
  /**
  * Operators overloads.
  */
  /*************************************************************************/

  float&
  Matrix4x4::operator()(const int32& row, const int32& column) {
    return this->m_mat4x4[row][column];
  }

  Matrix4x4 
  Matrix4x4::operator+(Matrix4x4& matrix)const {
    Matrix4x4 add(m_columnSize, m_rowSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        add(i, j) = this->m_mat4x4[i][j] + matrix(i, j);
      }
    }
    return add;
  }

  void 
  Matrix4x4::operator+=(Matrix4x4& matrix){
    *this = *this + matrix;
  }

  Matrix4x4 
  Matrix4x4::operator-(Matrix4x4& matrix)const {
    Matrix4x4 subs(m_columnSize, m_rowSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        subs(i, j) = this->m_mat4x4[i][j] - matrix(i, j);
      }
    }

    return subs;
  }

  Matrix4x4 
  Matrix4x4::operator*(Matrix4x4& matrix)const {
    Matrix4x4 multip(m_rowSize, matrix.getColumns());

    if (m_columnSize != matrix.getRows()) {
      //return "Error";
    }

    float temp = 0.0f;

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < matrix.getColumns(); ++j) {
        temp = 0.0f;

        for (int32 k = 0; k < m_columnSize; ++k) {
          temp += m_mat4x4[i][k] * matrix(k, j);
        }

        multip(i, j) = temp;
      }
    }

    return multip;
  }

  Matrix4x4
  Matrix4x4::operator+(float data)const {
    Matrix4x4 result(m_rowSize, m_columnSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        result(i, j) = this->m_mat4x4[i][j] + data;
      }
    }

    return result;
  }

  Matrix4x4 
  Matrix4x4::operator-(float data)const {
    Matrix4x4 result(m_rowSize, m_columnSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        result(i, j) = this->m_mat4x4[i][j] - data;
      }
    }

    return result;
  }

  Matrix4x4 
  Matrix4x4::operator*(float data)const {
    Matrix4x4 result(m_rowSize, m_columnSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        result(i, j) = this->m_mat4x4[i][j] * data;
      }
    }

    return result;
  }

  void 
  Matrix4x4::operator*=(float data) {
    *this = *this * data;
  }

  Matrix4x4 
  Matrix4x4::operator/(float data)const {
    Matrix4x4 result(m_rowSize, m_columnSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        result(i, j) = this->m_mat4x4[i][j] / data;
      }
    }

    return result;
  }



  /*************************************************************************/
  /**
  * Gets.
  */
  /*************************************************************************/

  int32
  Matrix4x4::getRows() const {
    return this->m_rowSize;
  }

  int32
  Matrix4x4::getColumns() const {
    return this->m_columnSize;
  }
}