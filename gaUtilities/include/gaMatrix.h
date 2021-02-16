#pragma once

#include <vector>

#include "gaPlatformTypes.h"

namespace gaEngineSDK{

  class GA_UTILITY_EXPORT Matrix
  {
    public:
      /*************************************************************************/
      /**
      * Constructor and Destructor
      */
      /*************************************************************************/
  
      /**
      * @brief Take the column size, row size and an initial value for each cell.
      *        This will let you create any matrix.
      */
      Matrix(int32 columnSize = 4, int32 rowSize = 4, float value = 0.0f);
  
      /**
      * @brief Copy the members of one class instance to another.
      */
      Matrix(const Matrix& matrix);
  
      ~Matrix() = default;
  
      /*************************************************************************/
      /**
      * Normal methods.
      */
      /*************************************************************************/
  
      /**
      * @brief Let us convert our row in column and column in row.
      */
      Matrix&
      transpose();
  
      /*************************************************************************/
      /**
      * Operators overloads.
      */
      /*************************************************************************/
  
      /**
      * @brief Let us add the data of one matrix with another one.
      */
      Matrix
      operator+(Matrix& matrix)const;
  
      /**
      * @brief Let us substract the data of one matrix with another one.
      */
      Matrix
      operator-(Matrix& matrix)const;
  
      /**
      * @brief Let us multiply the data of one matrix with another one.
      */
      Matrix
      operator*(Matrix& matrix)const;
  
      /**
      * @brief Let us add the data of one matrix with one data.
      */
      Matrix
      operator+(float data)const;
  
      /**
      * @brief Let us substract the data of one matrix with one data.
      */
      Matrix
      operator-(float data)const;
  
      /**
      * @brief Let us multiply the data of one matrix with one data.
      */
      Matrix
      operator*(float data)const;
  
      /**
      * @brief Let us divide the data of one matrix with one data.
      */
      Matrix
      operator/(float data)const;
  
      /**
      * @brief You can access to a specific position.
      */
      float&
      operator()(const int32& row, const int32& column);
  
      /*************************************************************************/
      /**
      * Gets.
      */
      /*************************************************************************/
  
      /**
      * @brief Get the value of the row.
      */
      int32
      getRows() const;
  
      /**
      * @brief Get the value of the column.
      */
      int32
      getColumns() const;
  
    private:
      int32 m_rowSize;
      int32 m_columnSize;
      std::vector<std::vector<float> > m_matrix;
  };
}