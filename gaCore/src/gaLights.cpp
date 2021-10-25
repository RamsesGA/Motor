#include "gaLights.h"
#include "gaGraphicsApi.h"
#include "gaActor.h"
#include "gaModels.h"
#include "gaResourceManager.h"
#include "gaStaticMesh.h"
#include "gaSceneGraph.h"

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

    m_pLightCamera->setView(createViewMatrix(m_pLightCamera->getCamEye(),
                                             m_pLightCamera->getLookAt(), 
                                             m_pLightCamera->getCamUp()));

    m_pLightCamera->setProjection(createOrtographicProyectionLH(1000.0f,
                                                                1000.0f,
                                                                0.01f,
                                                                5000.0f));
  }

  void
  Lights::update(const float& deltaTime) {
    auto transform = m_pMyActor->getComponent<Transform>();

    Vector3 eye = transform->getPosition();
    Vector3 lookAt = transform->getLookAt();
    Vector3 up = transform->getUp();

    m_pLightCamera->setEye(eye);
    m_pLightCamera->setLookAt(lookAt);
    m_pLightCamera->setUp(up);

    m_pLightCamera->setView(createViewMatrix(eye, lookAt, up));

    //TODO:someday change projection
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