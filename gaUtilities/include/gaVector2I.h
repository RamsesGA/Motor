#pragma once

#include<gaPlatformTypes.h>

using gaEngineSDK::int32;

/**
    * @brief
    */

class Vector2I 
{
  public:
    /**
    * @brief Constructor.
    */
    Vector2I() = default;

    /**
    * @brief Constructor to start a vector with x and y.
    */
    Vector2I(int32 x, int32 y);

    /**
    * @brief Destroyer.
    */
    ~Vector2I() = default;

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
    dotProduct(const Vector2I& vector)const;

    /**
    * @brief Operation between this vector with another.
    * @return Resultant vector perpendicular to the plane of these vectors.
    */
    Vector2I
    crossProduct(const Vector2I& vector)const;

    /*************************************************************************/
    /**
    * Operator overload.
    */
    /*************************************************************************/

    /**
    * @brief Operator overload of less.
    */
    bool
    operator <(const Vector2I& vector);

    /**
    * @brief Operator overload of less equal.
    */
    bool
    operator <=(const Vector2I& vector);

    /**
    * @brief Operator overload of higher.
    */
    bool
    operator >(const Vector2I& vector);

    /**
    * @brief Operator overload of higher equal.
    */
    bool
    operator >=(const Vector2I& vector);

    /**
    * @brief Operator overload of equal.
    */
    Vector2I&
    operator =(const Vector2I& vector);

    /**
    * @brief Operator overload of equal equal.
    */
    bool
    operator ==(const Vector2I& vector);

    /**
    * @brief Operator overload of sum.
    */
    Vector2I
    operator +(const Vector2I& vector)const;

    /**
    * @brief Operator overload of more equal.
    */
    Vector2I&
    operator +=(const Vector2I& vector);

    /**
    * @brief Operator overload of les
    */
    Vector2I
    operator -(const Vector2I & vector)const;

    /**
    * @brief Operator overload of
    */
    Vector2I&
    operator -=(const Vector2I& vector);

    /**
    * @brief Operator overload of
    */
    Vector2I
    operator *(const int32 vector)const;

    /**
    * @brief Operator overload of
    */
    Vector2I&
    operator *=(const int32 vector);

    /**
    * @brief Operator overload of
    */
    Vector2I
    operator /(const int32 vector)const;

    /**
    * @brief Operator overload of
    */
    Vector2I&
    operator /=(const int32 vector);

    /**
    * @brief Operator overload of
    */
    Vector2I
    operator *(const Vector2I& vector)const;

    /**
    * @brief Operator overload of
    */
    Vector2I
    operator %(const Vector2I& vector)const;

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

  private:
    /*************************************************************************/
    /**
    * Member.
    */
    /*************************************************************************/

    int32 m_x;
    int32 m_y;
};