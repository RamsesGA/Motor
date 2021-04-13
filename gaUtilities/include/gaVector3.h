#pragma once

#include "gaPrerequisitesUtilities.h"
#include "gaMath.h"

namespace gaEngineSDK {

  class GA_UTILITY_EXPORT Vector3
  {
    public:
      /**
      * @brief Constructor to start a vector with x and y.
      */
      Vector3(float x = 0.0f,
              float y = 0.0f,
              float z = 0.0f);

      /**
      * @brief Destroyer.
      */
      virtual
      ~Vector3() = default;
    
      /*************************************************************************/
      /**
      * Mathematical methods.
      */
      /*************************************************************************/
    
      /**
      * @brief Represents the length of a vector.
      */
      float
      magnitude();
    
      /**
      * @brief Vector with length of 1 unit.
      */
      void
      normalize();
    
      /**
      * @brief Operation between this vector with another.
      * @return Determine the angle between vectors.
      */
      float
      dotProduct(const Vector3& vector)const;
    
      /**
      * @brief Calculate the cross product of two vectors.
      * @param Another vector to calculate.
      * @return The cross product.
      */
      Vector3
      crossProduct(const Vector3& vector)const;
    
      /*************************************************************************/
      /**
      * Operator overload.
      */
      /*************************************************************************/
    
      /**
      * @brief Check if one vector is less than another, based on X, Y.
      * @param Another vector to compare.
      * @return We return true if it is small, otherwise it is false.
      */
      bool
      operator<(const Vector3& vector);
    
      /**
      * @brief Check if one vector is greater than another, based on X, Y.
      * @param Another vector to compare.
      * @return We return true if it is greater, otherwise it is false.
      */
      bool
      operator>(const Vector3& vector);
    
      /**
      * @brief We match the values of one vector in another.
      * @param Another vector to match.
      * @return New value for the vector.
      */
      Vector3&
      operator=(const Vector3& vector);
    
      /**
      * @brief Compares this vector against another
      * @param Another vector to compare.
      * @return True if the vectors are equal, otherwise false.
      */
      bool
      operator==(const Vector3& vector);
    
      /**
      * @brief We add the value of a saved vector and yours.
      * @param Another vector to sum.
      * @return New value for the vector.
      */
      Vector3
      operator+(const Vector3& vector)const;
    
      /**
      * @brief We add the value of your current vector and accept another vector.
      * @param Another vector to sum.
      * @return New value for the first vector.
      */
      Vector3&
      operator+=(const Vector3& vector);
    
      /**
      * @brief We subtract the value of one vector with another.
      * @param Another vector to rest.
      * @return New value for the vector.
      */
      Vector3
      operator-(const Vector3& vector)const;
    
      /**
      * @brief We subtract the value of the current vector with another.
      * @param Another vector to rest.
      * @return New value for the first vector.
      */
      Vector3&
      operator-=(const Vector3& vector);
    
      /**
      * @brief We multiply the value of one vector with an integer.
      * @param An integer to multiply.
      * @return New value for the vector.
      */
      Vector3
      operator*(const float val)const;
    
      /**
      * @brief We multiply the value of the current vector with an integer.
      * @param An integer to multiply.
      * @return New value for the first vector.
      */
      Vector3&
      operator*=(const float val);
    
      /**
      * @brief We multiply the value of one vector with another.
      * @param Another vector to multiply.
      * @return New value for the vector.
      */
      Vector3
      operator*(const Vector3& vector)const;

      /**
      * @brief Obtain a specific component.
      * @param index Specific position to enter in the vector.
      * @return Copy of that component.
      */
      float&
      operator[](uint32 index);

      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/
      float m_x;
      float m_y;
      float m_z;
  };
}