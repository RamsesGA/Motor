#pragma once

#include "gaPrerequisitesUtilities.h"
#include "gaRadians.h"

namespace gaEngineSDK {

  class GA_UTILITY_EXPORT PlatformMath
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and Destructor
    */
    /*************************************************************************/
    PlatformMath() = default;
    
    ~PlatformMath() = default;
    
    static float
    unwindDegrees(float A);
    
    static float
    unwindRadians(float A);
    
    static float
    fastSin(const float& x);
    
    static float
    fastCos(const float& x);
    
    template<typename T>
    static T abs(T val);
    
    static float 
    abs(float val);
    
    static float 
    invSqrt(float val);
    
    /*************************************************************************/
    /**
    * Trigonometric methods
    */
    /*************************************************************************/
    
    /**
    * @brief Returns the cosine of an angle of x.
    */
    template<typename T>
    static T 
    cos(T x);
    
    static float
    cos(float x);
    
    /**
    * @brief Returns the sine of an angle of x.
    */
    template<typename T>
    static T 
    sin(T x);
    
    static float
    sin(float x);
    
    /**
    * @brief Returns the tangent of an angle of x.
    */
    template<typename T>
    static T 
    tan(T x);
    
    static float
    tan(float x);
    
    /**
    * @brief Returns the principal value of the arc cosine of x,
    *        expressed in radians. In trigonometric,
    *        arc cosine is the inverse operation of cosine.
    */
    static float
    arcCos(float x);
    
    /**
    * @brief Returns the principal value of the arc sine of x,
    *        expressed in radians. In trigonometric,
    *        arc sine is the inverse operation of sine.
    */
    static float
    arcSin(float x);
    
    /**
    * @brief Returns the principal value of the arc tangent of x,
    *        expressed in radians. In trigonometrics,
    *        arc tangent is the inverse operation of tangent.
    */
    static float
    arcTan(float x);
    
    static
    float 
    arcTan(float val, float val2);
    
    /*************************************************************************/
    /**
    * Hyperbolic methods
    */
    /*************************************************************************/
    
    /**
    * @brief Returns the hyperbolic cosine of x.
    */
    static float
    cosHB(float x);
    
    /**
    * @brief Returns the hyperbolic sine of x.
    */
    static float
    sinHB(float x);
    
    /**
    * @brief Returns the hyperbolic tangent of x.
    */
    static float
    tanHB(float x);
    
    /**
    * @brief Returns the nonnegative area hyperbolic cosine of x.
    *        The area hyperbolic cosine is the inverse operation
    *        of the hyperbolic cosine.
    */
    static float
    arcCosHB(float x);
    
    /**
    * @brief Returns the area hyperbolic sine of x.
    *        The area hyperbolic sine is the inverse operation
    *        of the hyperbolic sine.
    */
    static float
    arcSinHB(float x);
    
    /**
    * @brief Returns the area hyperbolic tangent of x.
    *        The area hyperbolic tangent is the inverse operation
    *        of the hyperbolic tangent.
    */
    static float
    arcTanHB(float x);
    
    /*************************************************************************/
    /**
    * Exponential and logarithmic functions
    */
    /*************************************************************************/
    
    /**
    * @brief Returns the base-e exponential function of x,
    *        which is e raised to the power x: ex.
    */
    static float
    exp(float x);
    
    /**
    * @brief Returns the natural logarithm of x.
    */
    static float
    log(float x);
    
    /*************************************************************************/
    /**
    * Power functions
    */
    /*************************************************************************/
    
    /**
    * @brief Returns base raised to the power exponent.
    */
    static float
    pow(float data, float exponent);
    
    /**
    * @brief Returns the square root of x in floats.
    */
    static float
    sqrtf(float x);
    
    /**
    * @brief Returns the natural logarithm of x.
    */
    static float
    hypo(float x, float y);
    
    /*************************************************************************/
    /**
    * Members
    */
    /*************************************************************************/
    
    static const float PI;
    static const float EULER_NUMBER;
    static const float SMALL_NUMBER;
    static const float KINDA_SMALL_NUMBER;
    static const float BIG_NUMBER;
    static const float DELTA;
    static const float FOV;
    static const float DEG2RAD;
    static const float RAD2DEG;
    static const float EPSILONF;
    static const float TWO_PI;
  };
}