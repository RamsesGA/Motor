#include <gaGraphicsApi.h>

#include "gaLights.h"


namespace gaEngineSDK {

  Lights::Lights(Vector3 position, Vector3 lookAt) {
    auto myGraphicsApi = g_graphicApi().instancePtr();
    
    m_pLightCamera = make_shared<Camera>();
    m_pLightCamera->setEye(position);
    m_pLightCamera->setLookAt(lookAt);
    m_pLightCamera->setUp();
    m_pLightCamera->setFar();
    m_pLightCamera->setNear();
    m_pLightCamera->setFoV();
    m_pLightCamera->setWidth(myGraphicsApi->m_width);
    m_pLightCamera->setHeight(myGraphicsApi->m_height);

    m_pLightCamera->startCamera();

    Matrix4x4 mView = {-1.0f,   -0.01f,  0.01f,   0.0f,
                       -0.0f,   -0.16f, -0.99f,   0.0f,
                        0.01f,  -0.99f,  0.16f,   0.0f,
                       -19.41f, -81.41f, 330.27f, 1.0f };

    m_pLightCamera->setView(mView);

    //m_pLightCamera->setView(createViewMatrix(m_pLightCamera->getCamEye(),
    //                                         m_pLightCamera->getLookAt(), 
    //                                         m_pLightCamera->getCamUp()));

    m_pLightCamera->setProjection(createOrtographicProyectionLH(-500.0f,
                                                                500.0f,
                                                                -500.0f,
                                                                500.0f,
                                                                0.1f,
                                                                2000.0f));
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
    m_pLightCamera->setEye(Vector3(x, y, z));
  }

  void
  Lights::setLookAt(float x, float y, float z) {
    m_pLightCamera->setLookAt(Vector3(x, y, z));
  }

  void
  Lights::setIntensity(float value) {
    m_intensity = value;
  }

  void
  Lights::setEmissiveIntensity(float value) {
    m_emissiveIntensity = value;
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
    return m_pLightCamera->getCamEye();
  }

  Vector3 
  Lights::getLookAt() {
    return m_pLightCamera->getLookAt();
  }

  Matrix4x4
  Lights::getViewMatrix() {
    return m_pLightCamera->getView();
  }

  Matrix4x4
  Lights::getProjectionMatrix() {
    return m_pLightCamera->getProjection();
  }

  float
  Lights::getIntensity() {
    return m_intensity;
  }

  float
  Lights::getEmissiveIntensity() {
    return m_emissiveIntensity;
  }
}