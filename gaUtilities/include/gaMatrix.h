#pragma once

#include "gaPrerequisitesUtilities.h"
#include "gaVector3.h"
#include "gaVector2.h"

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
      * @brief Take the column size, row size and generates automatically
      *        identity matrix
      */
      Matrix(int32 columnSize, int32 rowSize);
  
      /**
      * @brief Copy the members of one class instance to another.
      */
      Matrix(const Matrix& matrix);

      /**
      * @brief Let you crate a matrix with only 3 vectors 3.
      */
      Matrix(const Vector3& vecX,
             const Vector3& vecY,
             const Vector3& vecZ);

      /**
      * @brief Let you crate a matrix with only 2 vectors 2.
      */
      Matrix(const Vector2& vecX,
             const Vector2& vecY);
  
      ~Matrix() = default;
  
      /*************************************************************************/
      /**
      * Normal methods.
      */
      /*************************************************************************/
  
      /**
      * @brief Let us convert our row in column and column in row.
      */
      Matrix
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

      /**
      * @brief Get the size of the matrix.
      */
      int32
      getSize() const;
      
  
    private:
      int32 m_rowSize;
      int32 m_columnSize;
      float m_mat3x3[3][3];
      float m_mat2x2[2][2];
      std::vector<std::vector<float> > m_matrix;
  };
}