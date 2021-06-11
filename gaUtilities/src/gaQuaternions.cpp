#include "gaQuaternions.h"
#include "gaDegrees.h"

namespace gaEngineSDK {
  Quaternions::Quaternions(const float& x, const float& y, const float& z) {
    m_x = x;
    m_y = y;
    m_z = z;
    m_w = 0.0f;
  }

  Quaternions::Quaternions(const Radians& x, const Radians& y, const Radians& z) {
    float fSinPitch = Math::taylorSin(x.valueRadians() * 0.5f);
    float fCosPitch = Math::taylorCos(x.valueRadians() * 0.5f);
    float fSinYaw = Math::taylorSin(y.valueRadians() * 0.5f);
    float fCosYaw = Math::taylorCos(y.valueRadians() * 0.5f);
    float fSinRoll = Math::taylorSin(z.valueRadians() * 0.5f);
    float fCosRoll = Math::taylorCos(z.valueRadians() * 0.5f);
    float fCosPitchCosYaw(fCosPitch * fCosYaw);
    float fSinPitchSinYaw(fSinPitch * fSinYaw);
    
    m_z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
    m_x = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
    m_y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
    m_w = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;
  }

  Quaternions::Quaternions(const Degrees& x, const Degrees& y, const Degrees& z) {
    float fSinPitch = Math::taylorSin(x.valueRadians() * 0.5f);
    float fCosPitch = Math::taylorCos(x.valueRadians() * 0.5f);
    float fSinYaw = Math::taylorSin(y.valueRadians() * 0.5f);
    float fCosYaw = Math::taylorCos(y.valueRadians() * 0.5f);
    float fSinRoll = Math::taylorSin(z.valueRadians() * 0.5f);
    float fCosRoll = Math::taylorCos(z.valueRadians() * 0.5f);
    
    float fCosPitchCosYaw(fCosPitch * fCosYaw);
    float fSinPitchSinYaw(fSinPitch * fSinYaw);
    
    m_z = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
    m_x = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
    m_y = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
    m_w = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;
  }

  Quaternions::Quaternions(Vector3 vec3, const float& w) {
    m_x = vec3.m_x;
    m_y = vec3.m_y;
    m_z = vec3.m_z;
    m_w = w;
  }

  Quaternions::Quaternions(const Quaternions& quaternion) {
    m_x = quaternion.m_x;
    m_y = quaternion.m_y;
    m_z = quaternion.m_z;
    m_w = quaternion.m_w;
  }

  Quaternions&
  Quaternions::operator=(const Quaternions& quaternion) {
    m_x = quaternion.m_x;
    m_y = quaternion.m_y;
    m_z = quaternion.m_z;
    m_w = quaternion.m_w;

    return *this;
  }

  Quaternions&
  Quaternions::operator+=(const Quaternions& quaternion) {
    m_x += quaternion.m_x;
    m_y += quaternion.m_y;
    m_z += quaternion.m_z;
    m_w += quaternion.m_w;

    return *this;
  }

  Quaternions
  Quaternions::operator+(const Quaternions& quaternion) const {
    Quaternions tmpQuaternion;
    
    tmpQuaternion.m_x = m_x + quaternion.m_x;
    tmpQuaternion.m_y = m_y + quaternion.m_y;
    tmpQuaternion.m_z = m_z + quaternion.m_z;
    tmpQuaternion.m_w = m_w + quaternion.m_w;

    return tmpQuaternion;
  }

  Quaternions&
  Quaternions::operator-=(const Quaternions& quaternion) {
    m_x -= quaternion.m_x;
    m_y -= quaternion.m_y;
    m_z -= quaternion.m_z;
    m_w -= quaternion.m_w;

    return *this;
  }

  Quaternions
  Quaternions::operator-(const Quaternions& quaternion) const {
    Quaternions tmpQuaternion;

    tmpQuaternion.m_x = m_x - quaternion.m_x;
    tmpQuaternion.m_y = m_y - quaternion.m_y;
    tmpQuaternion.m_z = m_z - quaternion.m_z;
    tmpQuaternion.m_w = m_w - quaternion.m_w;

    return tmpQuaternion;
  }

  Quaternions&
  Quaternions::operator*=(const Quaternions& quaternion) {
    Quaternions tmpQuaternion = *this * quaternion;
    *this = tmpQuaternion;

    return *this;
  }

  Quaternions
  Quaternions::operator*(const Quaternions& quaternion) const {
    Quaternions tmpQuaternion;
    
    tmpQuaternion.m_x = m_w * quaternion.m_x + m_x * quaternion.m_w + m_y * 
                              quaternion.m_z - m_z * quaternion.m_y;

    tmpQuaternion.m_y = m_w * quaternion.m_y + m_y * quaternion.m_w + m_z * 
                              quaternion.m_x - m_x * quaternion.m_z;

    tmpQuaternion.m_z = m_w * quaternion.m_z + m_z * quaternion.m_w + m_x * 
                              quaternion.m_y - m_y * quaternion.m_x;

    tmpQuaternion.m_w = m_w * quaternion.m_w - m_x * quaternion.m_x - m_y * 
                              quaternion.m_y - m_z * quaternion.m_z;
    
    return tmpQuaternion;
  }

  Quaternions&
  Quaternions::operator*=(const float& scale) {
    m_x *= scale;
    m_y *= scale;
    m_z *= scale;
    m_w *= scale;
    return *this;
  }

  Quaternions Quaternions::operator*(const float& scale) const {
    Quaternions tmpQuaternion;

    tmpQuaternion.m_x = m_x * scale;
    tmpQuaternion.m_y = m_y * scale;
    tmpQuaternion.m_z = m_z * scale;
    tmpQuaternion.m_w = m_w * scale;

    return tmpQuaternion;
  }

  bool
  Quaternions::operator==(const Quaternions& quaternion) const {
    if (Math::abs(m_x - quaternion.m_x) < (Math::EPSILONF) &&
        Math::abs(m_y - quaternion.m_y) < (Math::EPSILONF) &&
        Math::abs(m_z - quaternion.m_z) < (Math::EPSILONF) &&
        Math::abs(m_w - quaternion.m_w) < (Math::EPSILONF)) {

      return true;
    }

    return false;
  }

  bool
  Quaternions::operator!=(const Quaternions& quaternion) const {
    if (Math::abs(m_x - quaternion.m_x) > (Math::EPSILONF) ||
        Math::abs(m_y - quaternion.m_y) > (Math::EPSILONF) ||
        Math::abs(m_z - quaternion.m_z) > (Math::EPSILONF) ||
        Math::abs(m_w - quaternion.m_w) > (Math::EPSILONF)) {
      return true;
    }
    
    return false;
  }

  bool
  Quaternions::operator<(const Quaternions& quaternion) const {
    if (magnitude() < quaternion.magnitude()) {
      return true;
    }

    return false;
  }

  bool
  Quaternions::operator>(const Quaternions& quaternion) const {
    if (magnitude() > quaternion.magnitude()) {
      return true;
    }

    return false;
  }

  float
  Quaternions::magnitude() const {
    return Math::sqrtf(squareMagnitude());
  }

  float Quaternions::squareMagnitude() const {
    return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
  }

  Quaternions&
  Quaternions::normalize() {
    *this = getNormalize();

    return *this;
  }

  Quaternions
  Quaternions::getNormalize() const {
    float inverMag = squareMagnitude();

    GA_ASSERT(0.0f <= inverMag);
    
    inverMag = Math::invSqrt(inverMag);

    Quaternions tmpQuaternion;
    tmpQuaternion.m_x = m_x * inverMag;
    tmpQuaternion.m_y = m_y * inverMag;
    tmpQuaternion.m_z = m_z * inverMag;
    tmpQuaternion.m_w = m_w * inverMag;

    return tmpQuaternion;
  }

  Quaternions&
  Quaternions::conjugate() {
    *this = getConjugate();

    return *this;
  }

  Quaternions 
  Quaternions::getConjugate() const {
    float scalar = m_w;
    Vector3 tmpAxis = { m_x, m_y, m_z };

    tmpAxis *= -1;

    return Quaternions(tmpAxis, scalar);
  }

  Quaternions&
  Quaternions::inverse() {
    *this = getInverse();

    return *this;
  }

  Quaternions
  Quaternions::getInverse() const {
    float invSMagnitude = magnitude();

    invSMagnitude *= invSMagnitude;
    invSMagnitude = 1 / invSMagnitude;
    
    Quaternions tmpConjugate = getConjugate();
    
    float scalar = tmpConjugate.m_w * invSMagnitude;
    Vector3 tmpAxis = { tmpConjugate.m_x, tmpConjugate.m_y, tmpConjugate.m_z };

    tmpAxis *= invSMagnitude;

    return Quaternions(invSMagnitude, scalar);
  }

  Quaternions&
  Quaternions::rotate(const float& x, const float& y, const float& z, bool degree) {
    if (!degree) {
      return rotate(x * Math::RAD2DEG, y * Math::RAD2DEG, z * Math::RAD2DEG);
    }
    else {
      return rotate(x, y, z);
    }
  }
    
  Quaternions&
  Quaternions::rotate(Degrees x, Degrees y, Degrees z) {
    Quaternions q2(x, y, z), q = *this, qinv = q;
    qinv.conjugate();
    
    *this = q * q2 * qinv;

    return *this;
  }

  Vector3
  Quaternions::getEuler() {
    Vector3 euler;
    
    euler.m_z  = Math::arcTan(2 * (m_x * m_y + m_w * m_z), 
                              m_w * m_w + m_x * m_x - m_y * m_y - m_z * m_z);

    euler.m_z *= Math::RAD2DEG;

    euler.m_y  = Math::arcSin(-2 * (m_x * m_z - m_w * m_y));

    euler.m_y *= Math::RAD2DEG;

    euler.m_x  = Math::arcTan((2) * (m_y * m_z + m_w * m_x), 
                              m_w * m_w - m_x * m_x - m_y * m_y + m_z * m_z);

    euler.m_x *= Math::RAD2DEG;
    
    return euler;
  }

  Matrix4x4
  Quaternions::getMatrix() const {
    Matrix4x4 mat;

    mat.m_mat4x4[0][0] = 1.0f - 2.0f * (m_y * m_y + m_z * m_z);
    mat.m_mat4x4[1][0] = 2.0f * (m_x * m_y - m_z * m_w);
    mat.m_mat4x4[2][0] = 2.0f * (m_x * m_z + m_y * m_w); 
    mat.m_mat4x4[3][0] = 0;
    
    mat.m_mat4x4[0][1] = 2.0f * (m_x * m_y + m_z * m_w);
    mat.m_mat4x4[1][1] = 1.0f - 2.0f * (m_x * m_x + m_z * m_z);
    mat.m_mat4x4[2][1] = 2.0f * (m_y * m_z - m_x * m_w);
    mat.m_mat4x4[3][1] = 0;
    
    mat.m_mat4x4[0][2] = 2.0f * (m_x * m_z - m_y * m_w);
    mat.m_mat4x4[1][2] = 2.0f * (m_y * m_z + m_x * m_w);
    mat.m_mat4x4[2][2] = 1.0f - 2.0f * (m_x * m_x + m_y * m_y);
    mat.m_mat4x4[3][2] = 0;
    
    mat.m_mat4x4[0][3] = 0; 
    mat.m_mat4x4[1][3] = 0; 
    mat.m_mat4x4[2][3] = 0; 
    mat.m_mat4x4[3][3] = 1;

    return mat;
  }

}