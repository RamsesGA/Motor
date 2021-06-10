#pragma once

#include "gaPrerequisitesUtilities.h"
#include "gaVector3.h"
#include "gaMatrix4x4.h"
#include "gaRadians.h"

namespace gaEngineSDK {

  class GA_UTILITY_EXPORT Quaternions
  {
    public:
      /***********************************************************************/
      /**
     * Constructor and Destructor.
    */
      /***********************************************************************/
      Quaternions() = default;

      Quaternions(const float& x, const float& y, const float& z);

      Quaternions(const Radians& x, const Radians& y, const Radians& z);

      Quaternions(const Degrees& x, const Degrees& y, const Degrees& z);

      Quaternions(Vector3 vec3, const float& w);

      Quaternions(const Quaternions& quaternion);
    
      ~Quaternions() = default;

      /***********************************************************************/
      /**
     * Overload functions.
    */
      /***********************************************************************/

      /**
    * @brief equals a quaternion
    * @param quaternion is a quaternion what this with which is equaled
    * @return this quaternio equalize.
   */
      Quaternions&
      operator=(const Quaternions& quaternion);

      /**
      * @brief add two quaternions
      * @param quaternion is another quaternion.
      * @return the addition in this quaternion.
      */
      Quaternions&
      operator+=(const Quaternions& quaternion);

      /**
      * @brief add two quaternions
      * @param quaternion is another quaternion.
      * @return a quiaternion with the addition.
      */
      Quaternions
      operator+(const Quaternions& quaternion) const;

      /**
      * @brief subtract two quaternions.
      * @param quaternion is another quaternion.
      * @return a quiaternion with the subtraction in this quaternion.
      */
      Quaternions&
      operator-=(const Quaternions& quaternion);

      /**
      * @brief subtract two quaternions
      * @param quaternion is another quaternion.
      * @return a quiaternion with the subtraction.
      */
      Quaternions
      operator-(const Quaternions& quaternion) const;

      /**
      * @brief Multiply two quaternion.
      * @param quaternion Another quaternion.
      * @return the multiplication in this quaternion
      */
      Quaternions&
      operator*=(const Quaternions& quaternion);

      /**
      * @brief Multiply two quaternion.
      * @param quaternion Another quaternion.
      * @return a quaternion with the multiplication
      */
      Quaternions
      operator*(const Quaternions& quaternion) const;

      /**
      * @brief Multiply this quaternion with a scale
      * @param scale is the value to multiply the quaternion.
      * @return the multiplication in this quaternion
      */
      Quaternions&
      operator*=(const float& scale);

      /**
      * @brief Multiply this quaternion with a scale
      * @param scale is the value to multiply the quaternion.
      * @return a quaternion with the multiplication
      */
      Quaternions
      operator*(const float& scale) const;

      /**
      * @brief Compare two quaternions
      * @param quaternion is the quaternion with which it will be compared
      * @return true if this quaternion is identical to the other
      */
      bool
      operator==(const Quaternions& quaternion) const;

      /**
      * @brief Compare two quaternions
      * @param quaternion is the quaternion with which it will be compared
      * @return true if this quaternion is diferent to the other
      */
      bool
      operator!=(const Quaternions& quaternion) const;

      /**
      * @brief Compare two quaternions
      * @param quaternion is the quaternion with which it will be compared
      * @return true if this quaternion is less than the other
      */
      bool
      operator<(const Quaternions& quaternion) const;

      /**
      * @brief Compare two quaternions
      * @param quaternion is the quaternion with which it will be compared
      * @return true if this quaternion is greater than the other
      */
      bool
      operator>(const Quaternions& quaternion) const;

      /*****************************************************************************/
      /**
      * Functions
      */
      /*****************************************************************************/
      /**
      * @brief get the magnitud of this quaternion
      * @return the magnitude of this quaternion.
      */
      float
      magnitude() const;

      /**
      * @brief get the sauqre magnitud of this quaternion
      * @return the magnitude of this quaternion.
      */
      float
      squareMagnitude() const;

      /**
      * @brief tranform this quaternion to it's normalized form.
      * @return this quaternion normalized.
      */
      Quaternions&
      normalize();

      /**
      * @brief tranform this quaternion to it's normalized form.
      * @return None.
      */
      Quaternions
      getNormalize() const;

      /**
      * @brief conjugate this quaternion.
      * @return this quaternion conjugated.
      */
      Quaternions&
      conjugate();

      /**
      * @brief get the conjugate of this quaternion.
      * @return A quaternion with the conjugate of this quaternion.
      */
      Quaternions
      getConjugate() const;

      /**
      * @brief invert this quaternion.
      * @return this quaternion inverted.
      */
      Quaternions&
      inverse();

      /**
      * @brief get the inverse form of this quaternion.
      * @return A quaternion with the inverse form of this quaternion.
      */
      Quaternions
      getInverse() const;

      /**
      * @brief get to rotate this quaternion.
      * @return this quaternion rotated
      */
      Quaternions&
      rotate(const float& x, const float& y, const float& z, bool degree);

      /**
      * @brief get to rotate this quaternion.
      * @return this quaternion rotated
      */
      Quaternions&
      rotate(Degrees x, Degrees y, Degrees z);

      /**
      * @brief get to rotate this quaternion.
      * @return this quaternion rotated
      */
      Vector3
      getEuler();

      Matrix4x4
      getMatrix() const;
      
    public:
      /*****************************************************************************/
      /**
      /* Members
      /*/
      /*****************************************************************************/
      /**
      * @brief The quaternion's X-component.
      */
      float m_x;
      
      /**
      * @brief The quaternion's Y-component.
      */
      float m_y;
      
      /**
      * @brief The quaternion's Z-component.
      */
      float m_z;
      
      /**
      * @brief The quaternion's W-component.
      */
      float m_w;
  };
}