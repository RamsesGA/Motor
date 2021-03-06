#pragma once

#include "gaPrerequisitesUtilities.h"
#include "gaMath.h"

namespace gaEngineSDK {

  class GA_UTILITY_EXPORT Vector3I
  {
    public:
      /**
      * @brief Constructor to start a vector with x and y.
      */
      Vector3I(int32 x = 0,
               int32 y = 0,
               int32 z = 0);
    
      /**
      * @brief Destroyer.
      */
      ~Vector3I() = default;
    
      /***********************************************************************/
      /**
      * Mathematical normal methods.
      */
      /***********************************************************************/
    
      /**
      * @brief Operation between this vector with another.
      * @return Determine the angle between vectors.
      */
      int32
      dotProduct(const Vector3I& vector)const;
    
      /**
      * @brief Calculate the cross product of two vectors.
      * @param Another vector to calculate.
      * @return The cross product.
      */
      Vector3I
      crossProduct(const Vector3I& vector)const;
    
      /***********************************************************************/
      /**
      * Operator overload.
      */
      /***********************************************************************/
    
      /**
      * @brief Check if one vector is less than another, based on X, Y.
      * @param Another vector to compare.
      * @return We return true if it is small, otherwise it is false.
      */
      bool
      operator<(const Vector3I& vector);
    
      /**
      * @brief Check if one vector is greater than another, based on X, Y.
      * @param Another vector to compare.
      * @return We return true if it is greater, otherwise it is false.
      */
      bool
      operator>(const Vector3I& vector);
    
      /**
      * @brief We match the values of one vector in another.
      * @param Another vector to match.
      * @return New value for the vector.
      */
      Vector3I&
      operator=(const Vector3I& vector);
    
      /**
      * @brief Compares this vector against another
      * @param Another vector to compare.
      * @return True if the vectors are equal, otherwise false.
      */
      bool
      operator==(const Vector3I& vector);
    
      /**
      * @brief We add the value of a saved vector and yours.
      * @param Another vector to sum.
      * @return New value for the vector.
      */
      Vector3I
      operator+(const Vector3I& vector)const;
    
      /**
      * @brief We add the value of your current vector and accept another vector.
      * @param Another vector to sum.
      * @return New value for the first vector.
      */
      Vector3I&
      operator+=(const Vector3I& vector);
    
      /**
      * @brief We subtract the value of one vector with another.
      * @param Another vector to rest.
      * @return New value for the vector.
      */
      Vector3I
      operator-(const Vector3I& vector)const;
    
      /**
      * @brief We subtract the value of the current vector with another.
      * @param Another vector to rest.
      * @return New value for the first vector.
      */
      Vector3I&
      operator-=(const Vector3I& vector);
    
      /**
      * @brief We multiply the value of one vector with an integer.
      * @param An integer to multiply.
      * @return New value for the vector.
      */
      Vector3I
      operator*(const int32 val)const;
    
      /**
      * @brief We multiply the value of the current vector with an integer.
      * @param An integer to multiply.
      * @return New value for the first vector.
      */
      Vector3I&
      operator*=(const int32 val);
    
      /**
      * @brief We multiply the value of one vector with another.
      * @param Another vector to multiply.
      * @return New value for the vector.
      */
      Vector3I
      operator*(const Vector3I& vector)const;
    
      /***********************************************************************/
      /**
      * Member.
      */
      /***********************************************************************/
      int32 m_x;
      int32 m_y;
      int32 m_z;
  };
}