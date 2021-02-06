#pragma once

#include<gaPlatformTypes.h>

using gaEngineSDK::int32;

class Vector4I
{
  public:
    /**
    * @brief Constructor with default definition.
    */
    Vector4I(int32 x = 0,
             int32 y = 0,
             int32 z = 0,
             int32 w = 1);
  
    /**
    * @brief Constructor.
    */
    ~Vector4I() = default;
  
    /*************************************************************************/
    /**
    * Mathematical normal methods.
    */
    /*************************************************************************/
  
    /**
    * @brief Represents the length of a vector.
    */
    int32
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
    int32
    dotProduct(const Vector4I& vector)const;
  
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
    operator <(const Vector4I& vector);
  
    /**
    * @brief Check if one vector is less than another or equal, based on X, Y.
    * @param Another vector to compare.
    * @return We return true if it is small or equal, otherwise it is false.
    */
    bool
    operator <=(const Vector4I& vector);
  
    /**
    * @brief Check if one vector is greater than another, based on X, Y.
    * @param Another vector to compare.
    * @return We return true if it is greater, otherwise it is false.
    */
    bool
    operator >(const Vector4I& vector);
  
    /**
    * @brief Check if one vector is greater than another
             or equal, based on X, Y.
    * @param Another vector to compare.
    * @return We return true if it is greater or equal, otherwise it is false.
    */
    bool
    operator >=(const Vector4I& vector);
  
    /**
    * @brief We match the values of one vector in another.
    * @param Another vector to match.
    * @return New value for the vector.
    */
    Vector4I&
    operator =(const Vector4I& vector);
  
    /**
    * @brief Compares this vector against another
    * @param Another vector to compare.
    * @return True if the vectors are equal, otherwise false.
    */
    bool
    operator ==(const Vector4I& vector);
  
    /**
    * @brief We add the value of a saved vector and yours.
    * @param Another vector to sum.
    * @return New value for the vector.
    */
    Vector4I
    operator +(const Vector4I& vector)const;
  
    /**
    * @brief We add the value of your current vector and accept another vector.
    * @param Another vector to sum.
    * @return New value for the first vector.
    */
    Vector4I&
    operator +=(const Vector4I& vector);
  
    /**
    * @brief We subtract the value of one vector with another.
    * @param Another vector to rest.
    * @return New value for the vector.
    */
    Vector4I
    operator -(const Vector4I& vector)const;
  
    /**
    * @brief We subtract the value of the current vector with another.
    * @param Another vector to rest.
    * @return New value for the first vector.
    */
    Vector4I&
    operator -=(const Vector4I& vector);
  
    /**
    * @brief We multiply the value of one vector with an integer.
    * @param An integer to multiply.
    * @return New value for the vector.
    */
    Vector4I
    operator *(const int32 val)const;
  
    /**
    * @brief We multiply the value of the current vector with an integer.
    * @param An integer to multiply.
    * @return New value for the first vector.
    */
    Vector4I&
    operator *=(const int32 val);
  
    /**
    * @brief We divide the value of a vector with an integer.
    * @param An integer to divide.
    * @return New value for the vector.
    */
    Vector4I
    operator /(const int32 val)const;
  
    /**
    * @brief We divide the value of the current vector with an integer.
    * @param An integer to divide.
    * @return New value for the vector.
    */
    Vector4I&
    operator /=(const int32 val);
  
    /**
    * @brief We multiply the value of one vector with another.
    * @param Another vector to multiply.
    * @return New value for the vector.
    */
    Vector4I
    operator *(const Vector4I& vector)const;
  
    /*************************************************************************/
    /**
    * Sets.
    */
    /*************************************************************************/
  
    /**
    * @brief Method to save the member X.
    */
    void
    setX(int32 x);
  
    /**
    * @brief Member to save the member Y.
    */
    void
    setY(int32 y);
  
    /**
    * @brief Member to save the member Z.
    */
    void
    setZ(int32 z);
  
    /**
    * @brief Member to save the member Z.
    */
    void
    setW(int32 w);
  
    /*************************************************************************/
    /**
    * Gets.
    */
    /*************************************************************************/
  
    /**
    * @brief Method to obtain the member X.
    */
    int32
    getX();
  
    /**
    * @brief Member to obtain the member Y.
    */
    int32
    getY();
  
    /**
    * @brief Member to obtain the member Z.
    */
    int32
    getZ();
  
    /**
    * @brief Member to obtain the member Z.
    */
    int32
    getW();
  
  private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/
  
    int32 m_x;
    int32 m_y;
    int32 m_z;
    int32 m_w;
};