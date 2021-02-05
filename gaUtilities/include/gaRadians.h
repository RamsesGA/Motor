#pragma once

class Radians
{
  public:
    /*************************************************************************/
    /**
    * Constructor and Destructor
    */
    /*************************************************************************/
    Radians() = default;

    ~Radians() = default;

    template<typename type>
    type
    radiansToDegrees(type radian);

  
  private:
    /*************************************************************************/
    /**
    * Members
    */
    /*************************************************************************/
    float m_pi = 3.141592f;
};