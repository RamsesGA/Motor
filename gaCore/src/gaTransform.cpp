#include "gaTransform.h"

namespace gaEngineSDK {
  Transform::Transform() {
    m_typeComponents = TYPE_COMPONENTS::kTransform;

    m_rotation.x = 0.0f;
    m_rotation.y = 0.0f;
    m_rotation.z = 0.0f;
    m_rotation.w = 1.0f;

    m_position = (0.0f, 0.0f, 0.0f);
    m_scale = (0.0f, 0.0f, 0.0f);
  }

  Transform::Transform(const Quaternions& inRotation, const Vector3& inTranslation,
                       const Vector3& inScale3D) {
    m_rotation = (inRotation);
    m_position = (inTranslation);
    m_scale = (inScale3D);
  }

  Transform& Transform::operator=(const Transform& other) {
    this->m_rotation = other.m_rotation;
    this->m_position = other.m_position;
    this->m_scale = other.m_scale;
    return *this;
  }

  Transform 
  Transform::operator+(const Transform& atom) const {
    return Transform(m_rotation + atom.m_rotation, m_position + atom.m_position, 
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

  Transform 
  Transform::operator*(const Transform& other) const  { return Transform(); }

  void 
  Transform::operator*=(const Transform& other) { }

  Transform
  Transform::operator*(const Quaternions& other) const { return Transform(); }

  void 
  Transform::operator*=(const Quaternions& other) { }

  /*bool 
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