#pragma once

#include "gaPrerequisitesUtilities.h"
#include "gaVector3.h"
#include "gaMatrix4x4.h"
#include "gaQuaternions.h"

namespace gaEngineSDK {
  class GA_UTILITY_EXPORT Transform
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    Transform();
    
    Transform(const Quaternions& rotation, const Vector3& position, const Vector3& scale);
    
    ~Transform() = default;
    
    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/
    
    /*
    * @brief .
    */
    void
    editRotation(float x, float y, float z);
    
    /*
    * @brief .
    */
    void
    editPosition(float x, float y, float z);
    
    /*
    * @brief .
    */
    void
    editScale(float x, float y, float z);
    
    /*************************************************************************/
    /**
    * Overload operators.
    */
    /*************************************************************************/
    
    Transform&
    operator=(const Transform& other);
    
    Transform
    operator+(const Transform& atom) const;
    
    Transform&
    operator+=(const Transform& atom);
    
    Transform
    operator*(float mult) const;
    
    Transform&
    operator*=(float mult);
    
    Transform
    operator*(const Transform& other) const;
    
    void
    operator*=(const Transform& other);
    
    Transform
    operator*(const Quaternions& other) const;
    
    void
    operator*=(const Quaternions& other);
    
    /*
    bool
    operator==(const Transform& other) const;
    
    bool
    operator!=(const Transform& other) const;
    */
   private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/
    
    /*
    * @brief .
    */
    Vector3 m_position;
    
    /*
    * @brief .
    */
    Vector3 m_scale;

    /*
    * @brief .
    */
    Quaternions m_rotation;
    
    friend class Actor;
  };
}