
#include "gaMatrix.h"

namespace gaEngineSDK {

  Matrix::Matrix(int32 columnSize,
                 int32 rowSize,
                 float value) {
    m_columnSize = columnSize;
    m_rowSize = rowSize;
    m_matrix.resize(rowSize);

    for (int32 i = 0; i < m_matrix.size(); ++i) {
      m_matrix[i].resize(columnSize, value);
    }
  }

  Matrix::Matrix(const Matrix& matrix) {
    this->m_columnSize = matrix.getColumns();
    this->m_rowSize = matrix.getRows();
    this->m_matrix = matrix.m_matrix;
  }

  /*************************************************************************/
  /**
  * Normal methods.
  */
  /*************************************************************************/

  Matrix& Matrix::transpose() {
    Matrix trans(m_columnSize, m_rowSize, 0.0f);

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
    Matrix add(m_columnSize, m_rowSize, 0.0f);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        add(i, j) = this->m_matrix[i][j] + matrix(i, j);
      }
    }
    return add;
  }

  Matrix Matrix::operator-(Matrix& matrix)const {
    Matrix subs(m_columnSize, m_rowSize, 0.0);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        subs(i, j) = this->m_matrix[i][j] - matrix(i, j);
      }
    }

    return subs;
  }

  Matrix Matrix::operator*(Matrix& matrix)const {
    Matrix multip(m_rowSize, matrix.getColumns(), 0.0f);

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
    Matrix result(m_rowSize, m_columnSize, 0.0f);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        result(i, j) = this->m_matrix[i][j] + data;
      }
    }

    return result;
  }

  Matrix Matrix::operator-(float data)const {
    Matrix result(m_rowSize, m_columnSize, 0.0f);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        result(i, j) = this->m_matrix[i][j] - data;
      }
    }

    return result;
  }

  Matrix Matrix::operator*(float data)const {
    Matrix result(m_rowSize, m_columnSize, 0.0f);

    for (int32 i = 0; i < m_rowSize; ++i) {
      for (int32 j = 0; j < m_columnSize; ++j) {
        result(i, j) = this->m_matrix[i][j] * data;
      }
    }

    return result;
  }

  Matrix Matrix::operator/(float data)const {
    Matrix result(m_rowSize, m_columnSize, 0.0);

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
}