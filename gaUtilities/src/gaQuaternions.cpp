#include "gaQuaternions.h"
#include "gaDegrees.h"

namespace gaEngineSDK {
  Quaternions::Quaternions(const float& X, const float& Y, const float& Z, const float& W) {
    x = X;
    y = Y;
    z = Z;
    w = W;
  }

  Quaternions::Quaternions(const Radians& X, const Radians& Y, const Radians& Z, 
                           const Radians& W) {
    float fSinPitch = Math::fastSin(X.valueRadians() * 0.5f);
    float fCosPitch = Math::fastCos(X.valueRadians() * 0.5f);
    float fSinYaw = Math::fastSin(Y.valueRadians() * 0.5f);
    float fCosYaw = Math::fastCos(Y.valueRadians() * 0.5f);
    float fSinRoll = Math::fastSin(Z.valueRadians() * 0.5f);
    float fCosRoll = Math::fastCos(Z.valueRadians() * 0.5f);
    float fCosPitchCosYaw(fCosPitch * fCosYaw);
    float fSinPitchSinYaw(fSinPitch * fSinYaw);
    
    z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
    x = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
    y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
    w = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;
  }

  Quaternions::Quaternions(const Degrees& X, const Degrees& Y, const Degrees& Z, 
                           const Degrees& W) {
    float fSinPitch = Math::fastSin(X.valueRadians() * 0.5f);
    float fCosPitch = Math::fastCos(X.valueRadians() * 0.5f);
    float fSinYaw = Math::fastSin(Y.valueRadians() * 0.5f);
    float fCosYaw = Math::fastCos(Y.valueRadians() * 0.5f);
    float fSinRoll = Math::fastSin(Z.valueRadians() * 0.5f);
    float fCosRoll = Math::fastCos(Z.valueRadians() * 0.5f);
    
    float fCosPitchCosYaw(fCosPitch * fCosYaw);
    float fSinPitchSinYaw(fSinPitch * fSinYaw);
    
    z = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
    x = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
    y = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
    w = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;
  }

  Quaternions::Quaternions(Vector3 vec3, const float& W) {
    x = vec3.x;
    y = vec3.y;
    z = vec3.z;
    w = W;
  }

  Quaternions::Quaternions(const Quaternions& quaternion) {
    x = quaternion.x;
    y = quaternion.y;
    z = quaternion.z;
    w = quaternion.w;
  }

  Quaternions&
  Quaternions::operator=(const Quaternions& quaternion) {
    x = quaternion.x;
    y = quaternion.y;
    z = quaternion.z;
    w = quaternion.w;

    return *this;
  }

  Quaternions&
  Quaternions::operator+=(const Quaternions& quaternion) {
    x += quaternion.x;
    y += quaternion.y;
    z += quaternion.z;
    w += quaternion.w;

    return *this;
  }

  Quaternions
  Quaternions::operator+(const Quaternions& quaternion) const {
    Quaternions tmpQuaternion;
    
    tmpQuaternion.x = x + quaternion.x;
    tmpQuaternion.y = y + quaternion.y;
    tmpQuaternion.z = z + quaternion.z;
    tmpQuaternion.w = w + quaternion.w;

    return tmpQuaternion;
  }

  Quaternions&
  Quaternions::operator-=(const Quaternions& quaternion) {
    x -= quaternion.x;
    y -= quaternion.y;
    z -= quaternion.z;
    w -= quaternion.w;

    return *this;
  }

  Quaternions
  Quaternions::operator-(const Quaternions& quaternion) const {
    Quaternions tmpQuaternion;

    tmpQuaternion.x = x - quaternion.x;
    tmpQuaternion.y = y - quaternion.y;
    tmpQuaternion.z = z - quaternion.z;
    tmpQuaternion.w = w - quaternion.w;

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
    
    tmpQuaternion.x = w * quaternion.x + x * quaternion.w + y * 
                              quaternion.z - z * quaternion.y;

    tmpQuaternion.y = w * quaternion.y + y * quaternion.w + z * 
                              quaternion.x - x * quaternion.z;

    tmpQuaternion.z = w * quaternion.z + z * quaternion.w + x * 
                              quaternion.y - y * quaternion.x;

    tmpQuaternion.w = w * quaternion.w - x * quaternion.x - y * 
                              quaternion.y - z * quaternion.z;
    
    return tmpQuaternion;
  }

  Quaternions&
  Quaternions::operator*=(const float& scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    w *= scale;
    return *this;
  }

  Quaternions Quaternions::operator*(const float& scale) const {
    Quaternions tmpQuaternion;

    tmpQuaternion.x = x * scale;
    tmpQuaternion.y = y * scale;
    tmpQuaternion.z = z * scale;
    tmpQuaternion.w = w * scale;

    return tmpQuaternion;
  }

  bool
  Quaternions::operator==(const Quaternions& quaternion) const {
    if (Math::abs(x - quaternion.x) < (Math::EPSILONF) &&
        Math::abs(y - quaternion.y) < (Math::EPSILONF) &&
        Math::abs(z - quaternion.z) < (Math::EPSILONF) &&
        Math::abs(w - quaternion.w) < (Math::EPSILONF)) {

      return true;
    }

    return false;
  }

  bool
  Quaternions::operator!=(const Quaternions& quaternion) const {
    if (Math::abs(x - quaternion.x) > (Math::EPSILONF) ||
        Math::abs(y - quaternion.y) > (Math::EPSILONF) ||
        Math::abs(z - quaternion.z) > (Math::EPSILONF) ||
        Math::abs(w - quaternion.w) > (Math::EPSILONF)) {
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
    return x * x + y * y + z * z + w * w;
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
    tmpQuaternion.x = x * inverMag;
    tmpQuaternion.y = y * inverMag;
    tmpQuaternion.z = z * inverMag;
    tmpQuaternion.w = w * inverMag;

    return tmpQuaternion;
  }

  Quaternions&
  Quaternions::conjugate() {
    *this = getConjugate();

    return *this;
  }

  Quaternions 
  Quaternions::getConjugate() const {
    float scalar = w;
    Vector3 tmpAxis = { x, y, z };

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
    
    float scalar = tmpConjugate.w * invSMagnitude;
    Vector3 tmpAxis = { tmpConjugate.x, tmpConjugate.y, tmpConjugate.z };

    tmpAxis *= invSMagnitude;

    return Quaternions(invSMagnitude, scalar);
  }

  Quaternions&
  Quaternions::rotate(const float& X, const float& Y, const float& Z, bool degree) {
    if (!degree) {
      return rotate(X * Math::RAD2DEG, Y * Math::RAD2DEG, Z * Math::RAD2DEG);
    }
    else {
      return rotate(X, Y, Z);
    }
  }
    
  Quaternions&
  Quaternions::rotate(Degrees X, Degrees Y, Degrees Z) {
    Quaternions q2(X, Y, Z, 0.0f), q = *this, qinv = q;
    qinv.conjugate();
    
    *this = q * q2 * qinv;

    return *this;
  }

  Vector3
  Quaternions::getEuler() {
    Vector3 euler;
    
    euler.z  = Math::arcTan(2 * (x * y + w * z), 
                              w * w + x * x - y * y - z * z);

    euler.z *= Math::RAD2DEG;

    euler.y  = Math::arcSin(-2 * (x * z - w * y));

    euler.y *= Math::RAD2DEG;

    euler.x  = Math::arcTan((2) * (y * z + w * x), 
                              w * w - x * x - y * y + z * z);

    euler.x *= Math::RAD2DEG;
    
    return euler;
  }

  Matrix4x4
  Quaternions::getMatrix() const {
    Matrix4x4 mat;

    mat.m_mat4x4[0][0] = 1.0f - 2.0f * (y * y + z * z);
    mat.m_mat4x4[1][0] = 2.0f * (x * y - z * w);
    mat.m_mat4x4[2][0] = 2.0f * (x * z + y * w); 
    mat.m_mat4x4[3][0] = 0;
    
    mat.m_mat4x4[0][1] = 2.0f * (x * y + z * w);
    mat.m_mat4x4[1][1] = 1.0f - 2.0f * (x * x + z * z);
    mat.m_mat4x4[2][1] = 2.0f * (y * z - x * w);
    mat.m_mat4x4[3][1] = 0;
    
    mat.m_mat4x4[0][2] = 2.0f * (x * z - y * w);
    mat.m_mat4x4[1][2] = 2.0f * (y * z + x * w);
    mat.m_mat4x4[2][2] = 1.0f - 2.0f * (x * x + y * y);
    mat.m_mat4x4[3][2] = 0;
    
    mat.m_mat4x4[0][3] = 0; 
    mat.m_mat4x4[1][3] = 0; 
    mat.m_mat4x4[2][3] = 0; 
    mat.m_mat4x4[3][3] = 1;

    return mat;
  }

}