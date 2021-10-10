#pragma once

#include <gaVector3.h>
#include <gaMatrix4x4.h>
#include <gaQuaternions.h>

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT Transform : public Component
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
    * Inheritance.
    */
    /*************************************************************************/

    void
    update(const float& deltaTime) override;

    void
    render() override;

    /*************************************************************************/
    /**
    * Set.
    */
    /*************************************************************************/

    void
    setPosition(Vector3 pos);

    void
    setPosition(float x, float y, float z);

    void
    setScale(Vector3 scale);

    void
    setScale(float x, float y, float z);

    void
    setRotation(Quaternions rotation);

    void
    setRotation(float x, float y, float z);

    /*************************************************************************/
    /**
    * Get.
    */
    /*************************************************************************/

    Vector3
    getPosition();

    Vector3
    getScale();

    Matrix4x4
    getTransform();

    Quaternions
    getRotation();
    
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
    Matrix4x4 m_mTransform;

    /*
    * @brief .
    */
    Quaternions m_rotation;
    
    friend class Actor;
  };
}