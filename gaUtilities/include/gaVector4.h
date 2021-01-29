#pragma once

class Vector4
{
  public:
    /**
    * @brief Constructor.
    */
    Vector4() = default;

    /**
    * @brief Constructor with default definition.
    */
    Vector4(float x = 0.0f, 
            float y = 0.0f, 
            float z = 0.0f, 
            float w = 1.0f);

    /**
    * @brief Constructor.
    */
    ~Vector4() = default;

  private:
    float m_x;
    float m_y;
    float m_z;
    float m_w;
};