#pragma once

#include "gaPrerequisitesUtilities.h"

namespace gaEngineSDK {

  class GA_UTILITY_EXPORT Math
  {
    public:
      /*************************************************************************/
      /**
      * Constructor and Destructor
      */
      /*************************************************************************/
      Math() = default;
    
      ~Math() = default;
    
      /*************************************************************************/
      /**
      * Trigonometric methods
      */
      /*************************************************************************/
    
      /**
      * @brief Returns the cosine of an angle of x radians.
      */
      template<typename type>
      type&
      cos(type data);
    
      /**
      * @brief Returns the sine of an angle of x radians.
      */
      template<typename type>
      type&
      sin(type data);
    
      /**
      * @brief Returns the tangent of an angle of x radians.
      */
      template<typename type>
      type&
      tan(type data);
    
      /**
      * @brief Returns the principal value of the arc cosine of x,
      *        expressed in radians. In trigonometrics,
      *        arc cosine is the inverse operation of cosine.
      */
      template<typename type>
      type&
      arcCos(type data);
    
      /**
      * @brief Returns the principal value of the arc sine of x,
      *        expressed in radians. In trigonometrics,
      *        arc sine is the inverse operation of sine.
      */
      template<typename type>
      type&
      arcSin(type data);
    
      /**
      * @brief Returns the principal value of the arc tangent of x,
      *        expressed in radians. In trigonometrics,
      *        arc tangent is the inverse operation of tangent.
      */
      template<typename type>
      type&
      arcTan(type data);
    
      /*************************************************************************/
      /**
      * Hyperbolic methods
      */
      /*************************************************************************/
    
      /**
      * @brief Returns the hyperbolic cosine of x.
      */
      template<typename type>
      type&
      cosHB(type data);
    
      /**
      * @brief Returns the hyperbolic sine of x.
      */
      template<typename type>
      type&
      sinHB(type data);
    
      /**
      * @brief Returns the hyperbolic tangent of x.
      */
      template<typename type>
      type&
      tanHB(type data);
    
      /**
      * @brief Returns the nonnegative area hyperbolic cosine of x.
      *        The area hyperbolic cosine is the inverse operation
      *        of the hyperbolic cosine.
      */
      template<typename type>
      type&
      arcCosHB(type data);
    
      /**
      * @brief Returns the area hyperbolic sine of x.
      *        The area hyperbolic sine is the inverse operation
      *        of the hyperbolic sine.
      */
      template<typename type>
      type&
      arcSinHB(type data);
    
      /**
      * @brief Returns the area hyperbolic tangent of x.
      *        The area hyperbolic tangent is the inverse operation
      *        of the hyperbolic tangent.
      */
      template<typename type>
      type&
      arcTanHB(type data);
    
      /*************************************************************************/
      /**
      * Exponential and logarithmic functions
      */
      /*************************************************************************/
    
      /**
      * @brief Returns the base-e exponential function of x,
      *        which is e raised to the power x: ex.
      */
      template<typename type>
      type&
      exp(type data);
    
      /**
      * @brief Returns the natural logarithm of x.
      */
      template<typename type>
      type&
      log(type data);
    
      /*************************************************************************/
      /**
      * Power functions
      */
      /*************************************************************************/
    
      /**
      * @brief Returns base raised to the power exponent.
      */
      template<typename type>
      type&
      pow(type data, type exponent);
    
      /**
      * @brief Returns the square root of x in floats.
      */
      static float 
      sqrtf(float data);

      /**
      * @brief Returns the square root of x in integers.
      */
      static int32
      sqrti(int32 data);
    
      /**
      * @brief Returns the natural logarithm of x.
      */
      template<typename type>
      type&
      hypo(type x, type y);
  };
}