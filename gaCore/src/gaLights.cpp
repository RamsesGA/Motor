#include "gaLights.h"

namespace gaEngineSDK {
  void 
  Lights::generateViewMatrix() {
    Vector3 up;

    // Setup the vector that points upwards.
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    // Create the view matrix from the three vectors.
    m_viewMatrix = m_viewMatrix.matrixLookAtLH(&m_position, &m_lookAt, &up);
  }

  void 
  Lights::generateProjectionMatrix(float screenDepth, float screenNear) {
    float fieldOfView;
    float screenAspect;

    // Setup field of view and screen aspect for a square light source.
    fieldOfView = Math::PI / 2.0f;
    screenAspect = 1.0f;

    // Create the projection matrix for the light.
    m_projectionMatrix = m_projectionMatrix.perspectiveFovLH(fieldOfView, 
                                                             screenAspect, 
                                                             screenAspect,
                                                             screenNear,
                                                             screenDepth);
  }

  /***************************************************************************/
  /**
  * S E T S.
  */
  /***************************************************************************/
  void
  Lights::setAmbientColor(float r, float g, float b, float a) {
    m_ambientColor = Vector4(r,g,b,a);
  }

  void
  Lights::setDiffuseColor(float r, float g, float b, float a) {
    m_diffuseColor = Vector4(r, g, b, a);
  }

  void 
  Lights::setPosition(float x, float y, float z) {
    m_position = Vector3(x, y, z);
  }

  void
  Lights::setLookAt(float x, float y, float z) {
    m_lookAt.x = x;
    m_lookAt.y = y;
    m_lookAt.z = z;
  }

  /***************************************************************************/
  /**
  * G E T S.
  */
  /***************************************************************************/
  Vector4 
  Lights::getAmbientColor() {
    return m_ambientColor;
  }

  Vector4 
  Lights::getDiffuseColor() {
    return m_diffuseColor;
  }

  Vector3 
  Lights::getPosition() {
    return m_position;
  }

  void
  Lights::getViewMatrix(Matrix4x4& mView) {
    mView = m_viewMatrix;
  }

  void 
  Lights::getProjectionMatrix(Matrix4x4& mProjection) {
    mProjection = m_projectionMatrix;
  }
}