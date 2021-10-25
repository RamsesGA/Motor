#pragma once

#include "gaPrerequisitesUtilities.h"
#include "gaVector3.h"
#include "gaVector4.h"
#include "gaRadians.h"

namespace gaEngineSDK {
  class GA_UTILITY_EXPORT Matrix4x4
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and Destructor
    */
    /*************************************************************************/
    
    Matrix4x4();
    
    /**
    * @brief Define the value of the matrix in column major.
    */
    Matrix4x4(float x1, float y1, float z1, float w1,
              float x2, float y2, float z2, float w2,
              float x3, float y3, float z3, float w3,
              float x4, float y4, float z4, float w4);
    
    Matrix4x4(float x0, float x3, float x6,
              float x1, float x4, float x7,
              float x2, float x5, float x8);
    
    /**
    * @brief Copy the members of one class instance to another.
    */
    Matrix4x4(const Matrix4x4& matrix);
    
    /**
    * @brief Let you crate a matrix with only 3 vectors 3.
    */
    Matrix4x4(const Vector3& vecX,
              const Vector3& vecY,
              const Vector3& vecZ,
              const Vector3& vecW);
    
    ~Matrix4x4() = default;
    
    /*************************************************************************/
    /**
    * Normal methods.
    */
    /*************************************************************************/
    
    /**
    * @brief Let us convert our row in column and column in row.
    */
    Matrix4x4&
    transpose();
    
    /*
    * @brief .
    */
    Matrix4x4
    perspectiveFovLH(float FOV, float width, float height, float near, float farF);

    /*
    * @brief .
    */
    Matrix4x4
    matrixLookAtLH(const Vector3* pEye, const Vector3* pAt, const Vector3* pUp);
    
    /**
    * @brief Obtain a vector with the values of the matrix.
    * @return One vector with XYZ values.
    */
    Vector3
    getColumnMatrixInfo(uint32 index);
    
    /*
    * @brief Calculates x-axis rotation.
    */
    void
    rotationX(float speedRotation);
    
    /*
    * @brief Calculates x-axis rotation.
    */
    Matrix4x4&
    rotationX(const Radians& angleRadian);
    
    /*
    * @brief Calculates y-axis rotation.
    */
    void
    rotationY(float speedRotation);
    
    /*
    * @brief Calculates x-axis rotation.
    */
    Matrix4x4&
    rotationY(const Radians& angleRadian);
    
    /*
    * @brief Calculates z-axis rotation.
    */
    void
    rotationZ(float speedRotation);
    
    /*
    * @brief Calculates the axis with three vector.
    */
    void
    calculateAxis(Vector3 right, Vector3 up, Vector3 front);
    
    /*
    * @brief Calculates the position with the eye.
    */
    void
    calculatePosition(Vector3 eye);
    
    /**
    * @brief .
    */
    Matrix4x4&
    identity();
    
    /**
    * @brief .
    */
    Matrix4x4&
    scale(Vector3 scal);
    
    /**
    * @brief .
    */
    Matrix4x4&
    scale(float sx, float sy, float sz);
    
    /**
    * @brief .
    */
    Matrix4x4&
    translate(const Vector3& vec3);
    
    /**
    * @brief .
    */
    Matrix4x4&
    translate(float X, float Y, float Z);
    
    /**
    * @brief .
    */
    Matrix4x4
    invert();
    
    void
    adjoint(float tempMatrix[4][4], float adj[4][4]);
    
    /*************************************************************************/
    /**
    * Operators overloads.
    */
    /*************************************************************************/
    
    /**
    * @brief Let us add the data of one matrix with another one.
    */
    Matrix4x4
    operator+(Matrix4x4& matrix)const;
    
    void
    operator+=(Matrix4x4& matrix);
    
    /**
    * @brief Let us subs tract the data of one matrix with another one.
    */
    Matrix4x4
    operator-(Matrix4x4& matrix)const;
    
    /**
    * @brief Let us multiply the data of one matrix with another one.
    */
    Matrix4x4
    operator*(const Matrix4x4& matrix)const;
    
    /**
    * @brief Let us add the data of one matrix with one data.
    */
    Matrix4x4
    operator+(float data)const;
    
    /**
    * @brief Let us subs tract the data of one matrix with one data.
    */
    Matrix4x4
    operator-(float data)const;
    
    /**
    * @brief Let us multiply the data of one matrix with one data.
    */
    Matrix4x4
    operator*(float data)const;
    
    /**
    * @brief .
    */
    Matrix4x4&
    operator*=(const Matrix4x4& mat);
    
    /**
    * @brief .
    */
    void
    operator*=(float data);
    
    /**
    * @brief Let us divide the data of one matrix with one data.
    */
    Matrix4x4
    operator/(float data)const;
    
    /**
    * @brief You can access to a specific position.
    */
    float&
    operator()(const uint32& row, const uint32& column);
    
    /*************************************************************************/
    /**
    * Gets.
    */
    /*************************************************************************/
    
    /**
    * @brief Get the value of the row.
    */
    int32
    getRows() const;
    
    /**
    * @brief Get the value of the column.
    */
    int32
    getColumns() const;
    
    /**
    * @brief .
    */
    float
    getDeterminant(float tempMatrix[4][4], float n = 4);
    
    void
    getCofactor(float tempMatrix[4][4], float tempMatrix2[4][4], uint32 p, uint32 q, uint32 n);
    
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/
    
    float m_mat4x4[4][4];
  };

  GA_UTILITY_EXPORT extern Matrix4x4
  createViewMatrix(const Vector4& eye, const Vector4& lookAt, const Vector4& up);

  GA_UTILITY_EXPORT extern Matrix4x4
  createViewMatrix(const Vector3& eye, const Vector3& lookAt, const Vector3& up);

  GA_UTILITY_EXPORT extern Matrix4x4
  createOrtographicProyectionLH(const float& bottom, 
                                const float& top,
                                const float& left,
                                const float& right, 
                                const float& _near,
                                const float& _far);

  GA_UTILITY_EXPORT extern Matrix4x4
  createOrtographicProyectionLH(const float& width, 
                                const float& height, 
                                const float& _near,
                                const float& _far);
}