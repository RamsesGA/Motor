#pragma once

#include "gaPrerequisitesUtilities.h"
#include "gaMath.h"

namespace gaEngineSDK {
  class GA_UTILITY_EXPORT Vector4
  {
   public:
    /*************************************************************************/
    /*
    * Constructor and destructor.
    */
    /*************************************************************************/
    
    /**
    * @brief Constructor with default definition.
    */
    Vector4(float X = 0.0f, float Y = 0.0f, float Z = 0.0f, float W = 1.0f);
    
    ~Vector4() = default;
    
    /*************************************************************************/
    /**
    * Mathematical normal methods.
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
    dotProduct(const Vector4& vector)const;
    
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
    operator<(const Vector4& vector)const;
    
    /**
    * @brief Check if one vector is greater than another, based on X, Y.
    * @param Another vector to compare.
    * @return We return true if it is greater, otherwise it is false.
    */
    bool
    operator>(const Vector4& vector)const;
    
    /**
    * @brief We match the values of one vector in another.
    * @param Another vector to match.
    * @return New value for the vector.
    */
    Vector4&
    operator=(const Vector4& vector);
    
    /**
    * @brief Compares this vector against another
    * @param Another vector to compare.
    * @return True if the vectors are equal, otherwise false.
    */
    bool
    operator==(const Vector4& vector)const;
    
    /**
    * @brief We add the value of a saved vector and yours.
    * @param Another vector to sum.
    * @return New value for the vector.
    */
    Vector4
    operator+(const Vector4& vector)const;
    
    /**
    * @brief We add the value of your current vector and accept another vector.
    * @param Another vector to sum.
    * @return New value for the first vector.
    */
    Vector4&
    operator+=(const Vector4& vector);
    
    /**
    * @brief We subtract the value of one vector with another.
    * @param Another vector to rest.
    * @return New value for the vector.
    */
    Vector4
    operator-(const Vector4& vector)const;
    
    /**
    * @brief We subtract the value of the current vector with another.
    * @param Another vector to rest.
    * @return New value for the first vector.
    */
    Vector4&
    operator-=(const Vector4& vector);
    
    /**
    * @brief We multiply the value of one vector with an integer.
    * @param An integer to multiply.
    * @return New value for the vector.
    */
    Vector4
    operator*(const float val)const;
    
    /**
    * @brief We multiply the value of the current vector with an integer.
    * @param An integer to multiply.
    * @return New value for the first vector.
    */
    Vector4&
    operator*=(const float val);
    
    /**
    * @brief We multiply the value of one vector with another.
    * @param Another vector to multiply.
    * @return New value for the vector.
    */
    Vector4
    operator*(const Vector4& vector)const;
    
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/
    
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;
  };
}