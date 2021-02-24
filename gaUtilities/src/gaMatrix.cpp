
#include "gaMatrix.h"

namespace gaEngineSDK {

  Matrix::Matrix(int32 columnSize,
                 int32 rowSize) {
    m_columnSize = columnSize;
    m_rowSize = rowSize;
    m_matrix.resize(rowSize);

    for (int32 i = 0; i < m_matrix.size(); ++i) {
      m_matrix[i].resize(columnSize);
      for (int j = 0; j < m_rowSize; ++j) {
        if (i == j) {
          m_matrix[i][j] = 1.0f;
        }
        else {
          m_matrix[i][j] = 0.0f;
        }
      }
    }

    m_mat2x2[0][0] = 0.0f; m_mat2x2[0][1] = 0.0f;
    m_mat2x2[1][0] = 0.0f; m_mat2x2[1][1] = 0.0f;

    m_mat3x3[0][0] = 0.0f; m_mat3x3[0][1] = 0.0f; m_mat3x3[0][2] = 0.0f;
    m_mat3x3[1][0] = 0.0f; m_mat3x3[1][1] = 0.0f; m_mat3x3[1][2] = 0.0f;
    m_mat3x3[2][0] = 0.0f; m_mat3x3[2][1] = 0.0f; m_mat3x3[2][2] = 0.0f;
  }

  Matrix::Matrix(const Matrix& matrix) {
    this->m_columnSize = matrix.getColumns();
    this->m_rowSize = matrix.getRows();
    this->m_matrix = matrix.m_matrix;
  }

  Matrix::Matrix(const Vector3& vecX, 
                 const Vector3& vecY, 
                 const Vector3& vecZ){
    m_columnSize = 3;
    m_rowSize = 3;
    m_matrix.resize(m_rowSize);

    m_mat3x3[0][0] = vecX.m_x; m_mat3x3[0][1] = vecX.m_y; m_mat3x3[0][2] = vecX.m_z;
    m_mat3x3[1][0] = vecY.m_x; m_mat3x3[1][1] = vecY.m_y; m_mat3x3[1][2] = vecY.m_z;
    m_mat3x3[2][0] = vecZ.m_x; m_mat3x3[2][1] = vecZ.m_y; m_mat3x3[2][2] = vecZ.m_z;

    for (int32 i = 0; i < m_rowSize; ++i) {
      m_matrix[i].resize(m_columnSize);
      for (int32 j = 0; j < m_columnSize; ++j) {
        m_matrix[i][j] = m_mat3x3[i][j];
      }
    }

    m_mat2x2[0][0] = 0.0f; m_mat2x2[0][1] = 0.0f;
    m_mat2x2[1][0] = 0.0f; m_mat2x2[1][1] = 0.0f;
  }

  Matrix::Matrix(const Vector2& vecX, const Vector2& vecY){
    m_columnSize = 2;
    m_rowSize = 2;
    m_matrix.resize(m_rowSize);

    m_mat2x2[0][0] = vecX.m_x; m_mat2x2[0][1] = vecX.m_y;
    m_mat2x2[1][0] = vecY.m_x; m_mat2x2[1][1] = vecY.m_y;

    for (int32 i = 0; i < m_rowSize; ++i) {
      m_matrix[i].resize(m_columnSize);
      for (int32 j = 0; j < m_columnSize; ++j) {
        m_matrix[i][j] = m_mat2x2[i][j];
      }
    }

    m_mat3x3[0][0] = 0.0f; m_mat3x3[0][1] = 0.0f; m_mat3x3[0][2] = 0.0f;
    m_mat3x3[1][0] = 0.0f; m_mat3x3[1][1] = 0.0f; m_mat3x3[1][2] = 0.0f;
    m_mat3x3[2][0] = 0.0f; m_mat3x3[2][1] = 0.0f; m_mat3x3[2][2] = 0.0f;
  }

  /*************************************************************************/
  /**
  * Normal methods.
  */
  /*************************************************************************/

  Matrix Matrix::transpose() {
    Matrix trans(m_columnSize, m_rowSize);

    for (int32 i = 0; i < m_columnSize; ++i) {
      for (int32 j = 0; j < m_rowSize; ++j) {
        trans(i, j) = this->m_matrix[j][i];
      }
    }

    return trans;
  }

  /*************************************************************************/
  /**
  * Operators overloads.
  */
  /*************************************************************************/

  Matrix Matrix::operator+(Matrix& matrix)const {
    Matrix add(m_columnSize, m_rowSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        add(i, j) = this->m_matrix[i][j] + matrix(i, j);
      }
    }
    return add;
  }

  Matrix Matrix::operator-(Matrix& matrix)const {
    Matrix subs(m_columnSize, m_rowSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        subs(i, j) = this->m_matrix[i][j] - matrix(i, j);
      }
    }

    return subs;
  }

  Matrix Matrix::operator*(Matrix& matrix)const {
    Matrix multip(m_rowSize, matrix.getColumns());

    if (m_columnSize != matrix.getRows()) {
      //return "Error";
    }

    float temp = 0.0f;

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < matrix.getColumns(); ++j) {
        temp = 0.0f;

        for (int32 k = 0; k < m_columnSize; ++k) {
          temp += m_matrix[i][k] * matrix(k, j);
        }

        multip(i, j) = temp;
      }
    }

    return multip;
  }

  Matrix Matrix::operator+(float data)const {
    Matrix result(m_rowSize, m_columnSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        result(i, j) = this->m_matrix[i][j] + data;
      }
    }

    return result;
  }

  Matrix Matrix::operator-(float data)const {
    Matrix result(m_rowSize, m_columnSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        result(i, j) = this->m_matrix[i][j] - data;
      }
    }

    return result;
  }

  Matrix Matrix::operator*(float data)const {
    Matrix result(m_rowSize, m_columnSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        result(i, j) = this->m_matrix[i][j] * data;
      }
    }

    return result;
  }

  Matrix Matrix::operator/(float data)const {
    Matrix result(m_rowSize, m_columnSize);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        result(i, j) = this->m_matrix[i][j] / data;
      }
    }

    return result;
  }

  float& Matrix::operator()(const int32& row, const int32& column) {
    return this->m_matrix[row][column];
  }

  /*************************************************************************/
  /**
  * Gets.
  */
  /*************************************************************************/

  int32 Matrix::getRows() const {
    return this->m_rowSize;
  }

  int32 Matrix::getColumns() const {
    return this->m_columnSize;
  }

  int32 Matrix::getSize() const {
    return (m_columnSize * m_rowSize);
  }
}