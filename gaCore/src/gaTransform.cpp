#include <gaDegrees.h>

#include "gaTransform.h"
#include "gaActor.h"

namespace gaEngineSDK {
  Transform::Transform() {
    m_rotation.x = 0.0f; 
    m_rotation.y = 0.0f; 
    m_rotation.z = 0.0f; 
    m_rotation.w = 1.0f;
    m_position = Vector3(0.0f, 0.0f, 0.0f);
    m_scale = Vector3(1.0f, 1.0f, 1.0f);
  }

  Transform::Transform(const Quaternions& rotation, 
                       const Vector3& position,
                       const Vector3& scale) {
    m_rotation = (rotation); 
    m_position = (position); 
    m_scale = (scale);
  }

  /***************************************************************************/
  /**
  * Inheritance.
  */
  /***************************************************************************/

  void 
  Transform::update(const float& deltaTime) {
    Matrix4x4 mTemp;

    if (nullptr != m_pMyActor->m_pParent) {
      auto myParent = m_pMyActor->m_pParent;
      auto parentTransform = m_pMyActor->getComponent<Transform>();

      mTemp = parentTransform->m_mTransform;
    }

    //We avoid the value 0
    if (0 == m_scale.x) {
      m_scale.x = 0.00001f;
    }
    if (0 == m_scale.y) {
      m_scale.y = 0.00001f;
    }
    if (0 == m_scale.z) {
      m_scale.z = 0.00001f;
    }

    Matrix4x4 mScale = { m_scale.x,  0.0f,      0.0f,      0.0f,
                         0.0f,       m_scale.y, 0.0f,      0.0f,
                         0.0f,       0.0f,      m_scale.z, 0.0f,
                         0.0f,       0.0f,      0.0f,      1.0f };

    Matrix4x4 mPosition = {1.0f, 0.0f, 0.0f, m_position.x,
                           0.0f, 1.0f, 0.0f, m_position.y,
                           0.0f, 0.0f, 1.0f, m_position.z,
                           0.0f, 0.0f, 0.0f, 1.0f };

    m_rotation = Quaternions(Degrees(m_eulerRot.x), 
                             Degrees(m_eulerRot.y),
                             Degrees(m_eulerRot.z),
                             Degrees(0));

    Matrix4x4 mMultiplication = mPosition * mScale;

    mMultiplication *= m_rotation.getMatrix().transpose();

    m_mTransform = mMultiplication.transpose() * mTemp;

    m_mLocalTrans = mMultiplication.transpose();

    m_worldPosition = { m_mTransform(3, 0), m_mTransform(3, 1), m_mTransform(3, 2) };

    //
    m_front = { m_mTransform(0, 2), m_mTransform(1, 2), m_mTransform(2, 2) };
    m_front.normalize();

    m_right = { m_mTransform(0, 0), m_mTransform(1, 0), m_mTransform(2, 0) };
    m_right.normalize();

    m_up = { m_mTransform(0, 1), m_mTransform(1, 1), m_mTransform(2, 1) };
    m_up.normalize();
  }

  void 
  Transform::render() {

  }

  /***************************************************************************/
  /**
  * Sets.
  */
  /***************************************************************************/

  void
  Transform::setPosition(const Vector3& pos) {
    m_position = pos;
  }

  void
  Transform::setPosition(const float& x, const float& y, const float& z) {
    m_position.x = x;
    m_position.y = y; 
    m_position.z = z;
  }

  void 
  Transform::setRotation(const Quaternions& rotation) {
    m_rotation = rotation;
    m_eulerRot = m_rotation.getEuler();
  }

  void
  Transform::setRotation(const float& x, const float& y, const float& z) {
    m_rotation.x = x;
    m_rotation.y = y;
    m_rotation.z = z;
  }

  void 
  Transform::setEulerRotation(const Vector3& rot) {
    m_eulerRot = rot;
    if (m_eulerRot.x < 0) {
      m_eulerRot.x = 360.0f;
    }
    else if (m_eulerRot.x > 360.0f) {
      m_eulerRot.x = 0.0f;
    }
    if (m_eulerRot.y < 0) {
      m_eulerRot.y = 360.0f;
    }
    else if (m_eulerRot.y > 360.0f) {
      m_eulerRot.y = 0.0f;
    }
    if (m_eulerRot.z < 0) {
      m_eulerRot.z = 360.0f;
    }
    else if (m_eulerRot.z > 360.0f) {
      m_eulerRot.z = 0.0f;
    }
  }

  void
  Transform::setScale(const Vector3& scale) {
    m_scale = scale;
  }

  void
  Transform::setScale(const float& x, const float& y, const float& z) {
    m_scale.x = x; 
    m_scale.y = y; 
    m_scale.z = z;
  }
  
  /***************************************************************************/
  /**
  * Gets.
  */
  /***************************************************************************/

  Vector3
  Transform::getPosition() {
    return m_position;
  }

  Vector3 
  Transform::getScale() {
    return m_scale;
  }

  Matrix4x4
  Transform::getTransform() {
    return m_mTransform;
  }

  Quaternions 
  Transform::getRotation() {
    return m_rotation;
  }

  Vector3 
  Transform::getEulerRotation() {
    return m_eulerRot;
  }

  Vector3 
  Transform::getFront() {
    return m_front;
  }

  Vector3 
  Transform::getRight() {
    return m_right;
  }

  Vector3 
  Transform::getUp() {
    return m_up;
  }

  Vector3 
  Transform::getLookAt() {
    return (m_position + m_front);
  }

  /***************************************************************************/
  /**
  * Overload operators.
  */
  /***************************************************************************/

  Transform& Transform::operator=(const Transform& other) {
    this->m_rotation = other.m_rotation;
    this->m_position = other.m_position;
    this->m_scale = other.m_scale;
    return *this;
  }

  Transform
  Transform::operator+(const Transform& atom) const {
    return Transform(m_rotation + atom.m_rotation, 
                     m_position + atom.m_position, 
                     m_scale + atom.m_scale);
  }

  Transform&
  Transform::operator+=(const Transform& atom) {
    m_position += atom.m_position;

    m_rotation.x += atom.m_rotation.x;
    m_rotation.y += atom.m_rotation.y;
    m_rotation.z += atom.m_rotation.z;
    m_rotation.w += atom.m_rotation.w;

    m_scale += atom.m_scale;

    return *this;
  }

  Transform
  Transform::operator*(float mult) const {
    return Transform(m_rotation * mult, m_position * mult, m_scale * mult);
  }

  Transform&
  Transform::operator*=(float mult) {
    m_position *= mult;
    m_rotation.x *= mult;
    m_rotation.y *= mult;
    m_rotation.z *= mult;
    m_rotation.w *= mult;
    m_scale *= mult;

    return *this;
  }

  /*
  Transform
  Transform::operator*(const Transform& other) const  { return Transform(); }

  void
  Transform::operator*=(const Transform& other) { }

  Transform
  Transform::operator*(const Quaternions& other) const { return Transform(); }

  void
  Transform::operator*=(const Quaternions& other) { }

  bool
  Transform::operator==(const Transform& other) const {
    return (m_rotation == other.m_rotation) && (m_position == other.m_position) &&
           (m_scale == other.m_scale);
  }

  bool
  Transform::operator!=(const Transform& other) const {
    return (m_rotation != other.m_rotation) || (m_position != other.m_position) ||
           (m_scale != other.m_scale);
  }*/
}