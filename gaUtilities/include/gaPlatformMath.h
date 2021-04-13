#pragma once

#include "gaPrerequisitesUtilities.h"

namespace gaEngineSDK {

  class GA_UTILITY_EXPORT PlatformMath
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and Destructor
      */
      /***********************************************************************/
      PlatformMath() = default;
    
      virtual
      ~PlatformMath() = default;
    
      /***********************************************************************/
      /**
      * Trigonometric methods
      */
      /***********************************************************************/
    
      /**
      * @brief Returns the cosine of an angle of x radians.
      */
      static float
      cos(float data);
    
      /**
      * @brief Returns the sine of an angle of x radians.
      */
      static float
      sin(float data);
    
      /**
      * @brief Returns the tangent of an angle of x radians.
      */
      static float
      tan(float data);
    
      /**
      * @brief Returns the principal value of the arc cosine of x,
      *        expressed in radians. In trigonometrics,
      *        arc cosine is the inverse operation of cosine.
      */
      static float
      arcCos(float data);
    
      /**
      * @brief Returns the principal value of the arc sine of x,
      *        expressed in radians. In trigonometrics,
      *        arc sine is the inverse operation of sine.
      */
      static float
      arcSin(float data);
    
      /**
      * @brief Returns the principal value of the arc tangent of x,
      *        expressed in radians. In trigonometrics,
      *        arc tangent is the inverse operation of tangent.
      */
      static float
      arcTan(float data);
    
      /***********************************************************************/
      /**
      * Hyperbolic methods
      */
      /***********************************************************************/
    
      /**
      * @brief Returns the hyperbolic cosine of x.
      */
      static float
      cosHB(float data);
    
      /**
      * @brief Returns the hyperbolic sine of x.
      */
      static float
      sinHB(float data);
    
      /**
      * @brief Returns the hyperbolic tangent of x.
      */
      static float
      tanHB(float data);
    
      /**
      * @brief Returns the nonnegative area hyperbolic cosine of x.
      *        The area hyperbolic cosine is the inverse operation
      *        of the hyperbolic cosine.
      */
      static float
      arcCosHB(float data);
    
      /**
      * @brief Returns the area hyperbolic sine of x.
      *        The area hyperbolic sine is the inverse operation
      *        of the hyperbolic sine.
      */
      static float
      arcSinHB(float data);
    
      /**
      * @brief Returns the area hyperbolic tangent of x.
      *        The area hyperbolic tangent is the inverse operation
      *        of the hyperbolic tangent.
      */
      static float
      arcTanHB(float data);
    
      /***********************************************************************/
      /**
      * Exponential and logarithmic functions
      */
      /***********************************************************************/
    
      /**
      * @brief Returns the base-e exponential function of x,
      *        which is e raised to the power x: ex.
      */
      static float
      exp(float data);
    
      /**
      * @brief Returns the natural logarithm of x.
      */
      static float
      log(float data);
    
      /***********************************************************************/
      /**
      * Power functions
      */
      /***********************************************************************/
    
      /**
      * @brief Returns base raised to the power exponent.
      */
      static float
      pow(float data, float exponent);
    
      /**
      * @brief Returns the square root of x in floats.
      */
      static float
      sqrtf(float data);
    
      /**
      * @brief Returns the natural logarithm of x.
      */
      static float
      hypo(float x, float y);
    
      /***********************************************************************/
      /**
      * Members
      */
      /***********************************************************************/

      static const float PI;
      static const float EULER_NUMBER;
      static const float SMALL_NUMBER;
      static const float KINDA_SMALL_NUMBER;
      static const float BIG_NUMBER;
      static const float DELTA;
  };
}