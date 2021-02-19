#pragma once

#include <vector>

#include "gaPlatformTypes.h"
#include "gaVector3.h"

namespace gaEngineSDK {

  class GA_UTILITY_EXPORT Matrix4x4
  {
    public:
      /*************************************************************************/
      /**
      * Constructor and Destructor
      */
      /*************************************************************************/
    
      Matrix4x4(int32 columnSize, int32 rowSize);

      /**
      * @brief Define the value of the matrix in column major.
      */
      Matrix4x4(float x1, float y1, float z1, float w1,
                float x2, float y2, float z2, float w2, 
                float x3, float y3, float z3, float w3, 
                float x4, float y4, float z4, float w4);
    
      /**
      * @brief Copy the members of one class instance to another.
      */
      Matrix4x4(const Matrix4x4& matrix);

      /**
      * @brief Let you crate a matrix with only 3 vectors 3.
      */
      Matrix4x4(const Vector3& vecX,
                const Vector3& vecY,
                const Vector3& vecZ,
                const Vector3& vecW);
    
      ~Matrix4x4() = default;
    
      /*************************************************************************/
      /**
      * Normal methods.
      */
      /*************************************************************************/
    
      /**
      * @brief Let us convert our row in column and column in row.
      */
      Matrix4x4&
      transpose();
    
      /*************************************************************************/
      /**
      * Operators overloads.
      */
      /*************************************************************************/
    
      /**
      * @brief Let us add the data of one matrix with another one.
      */
      Matrix4x4
      operator+(Matrix4x4& matrix)const;

      void
      operator+=(Matrix4x4& matrix);
    
      /**
      * @brief Let us substract the data of one matrix with another one.
      */
      Matrix4x4
      operator-(Matrix4x4& matrix)const;
    
      /**
      * @brief Let us multiply the data of one matrix with another one.
      */
      Matrix4x4
      operator*(Matrix4x4& matrix)const;
    
      /**
      * @brief Let us add the data of one matrix with one data.
      */
      Matrix4x4
      operator+(float data)const;
    
      /**
      * @brief Let us substract the data of one matrix with one data.
      */
      Matrix4x4
      operator-(float data)const;
    
      /**
      * @brief Let us multiply the data of one matrix with one data.
      */
      Matrix4x4
      operator*(float data)const;

      void
      operator*=(float data);
    
      /**
      * @brief Let us divide the data of one matrix with one data.
      */
      Matrix4x4
      operator/(float data)const;
    
      /**
      * @brief You can access to a specific position.
      */
      float&
      operator()(const int32 & row, const int32 & column);
    
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
      float m_mat4x4[4][4];
  };
}