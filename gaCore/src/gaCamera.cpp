#include "gaCamera.h"
#include "gaRadians.h"

namespace gaEngineSDK {

  void
  Camera::startCamera() {
    createView();
    createProjectionMatrix();
    updateViewMatrix();
  }

  void
  Camera::updateViewMatrix() {
    m_right = m_view.getColumnMatrixInfo(0);

    m_up = m_view.getColumnMatrixInfo(1);

    m_front = m_view.getColumnMatrixInfo(2);

    m_cameraDesc.camLookAt = m_cameraDesc.camEye + m_front;
  }

  void
  Camera::inputDetection(KEYBOARD::E input, const float& deltaTime) {
    if ((KEYBOARD::kUP == input) || (KEYBOARD::kDOWN == input)) {
      pitchX(input, deltaTime);
    }
    if ((KEYBOARD::kRIGHT == input) || (KEYBOARD::kLEFT == input)) {
      rollZ(input, deltaTime);
    }
    if ((KEYBOARD::kZ == input) || (KEYBOARD::kC == input)) {
      yawY(input, deltaTime);
    }
    else {
      move(input, deltaTime);
    }
  }

  void
  Camera::pitchX(KEYBOARD::E input, const float& deltaTime) {
    Matrix4x4 rotation;
    float speedrot = 1.5f * deltaTime;

    if (KEYBOARD::kUP == input) {
      rotation.rotationX(speedrot);
    }
    else if (KEYBOARD::kDOWN == input) {
      rotation.rotationX(-speedrot);
    }

    m_view = rotation * m_view;

    updateViewMatrix();
  }

  void
  Camera::rollZ(KEYBOARD::E input, const float& deltaTime) {
    Matrix4x4 rotation;
    float speedrot = 3.5f * deltaTime;

    if (KEYBOARD::kRIGHT == input) {
      rotation.rotationZ(-speedrot);
    }
    else if (KEYBOARD::kLEFT == input) {
      rotation.rotationZ(speedrot);
    }

    m_view = rotation * m_view;

    updateViewMatrix();
  }

  void
  Camera::yawY(KEYBOARD::E input, const float& deltaTime) {
    Matrix4x4 rotation;
    float speedrot = 1.5f * deltaTime;

    if (KEYBOARD::kZ == input) {
      rotation.rotationY(speedrot);
    }
    else if (KEYBOARD::kC == input) {
      rotation.rotationY(-speedrot);
    }

    m_view = m_view * rotation;

    updateViewMatrix();
  }

  void
  Camera::move(KEYBOARD::E input, const float& deltaTime) {
    float speedMove = 450.0f * deltaTime;

    if (KEYBOARD::kW == input) {
      m_cameraDesc.camEye += m_front * speedMove;
    }
    else if (KEYBOARD::kS == input) {
      m_cameraDesc.camEye -= m_front * speedMove;
    }
    else if (KEYBOARD::kA == input) {
      m_cameraDesc.camEye -= m_right * speedMove;
    }
    else if (KEYBOARD::kD == input) {
      m_cameraDesc.camEye += m_right * speedMove;
    }
    else if (KEYBOARD::kQ == input) {
      m_cameraDesc.camEye += m_up * speedMove;
    }
    else if (KEYBOARD::kE == input) {
      m_cameraDesc.camEye -= m_up * speedMove;
    }

    m_axis.calculateAxis(m_right, m_up, m_front);

    m_position.calculatePosition(m_cameraDesc.camEye);
    m_position = m_position * m_axis;

    m_view = m_position;

    updateViewMatrix();
  }

  void
  Camera::mouseRotation(const float& deltaTime) {
    Vector2 firstPos;
    Vector2 secondPos;

    float speedRot = 2.30f * deltaTime;

    POINT Temp;
    GetCursorPos(&Temp);

    firstPos.x = (float)Temp.x;
    secondPos.y = (float)Temp.y;

    if (firstPos.x < m_originalMousePos.x) {
      m_cameraDesc.camLookAt -= m_right * speedRot;
      m_cameraDesc.camUp = m_up;
      createView();
      updateViewMatrix();
    }

    if (firstPos.x > m_originalMousePos.x) {
      m_cameraDesc.camLookAt += m_right * speedRot;
      m_cameraDesc.camUp = m_up;
      createView();
      updateViewMatrix();
    }

    if (secondPos.y < m_originalMousePos.y) {
      Matrix4x4 rotation;
      rotation.rotationX(speedRot);
      m_view *= rotation;
      
      updateViewMatrix();
    }

    if (secondPos.y > m_originalMousePos.y) {
      Matrix4x4 rotation;
      rotation.rotationX(-speedRot);
      m_view *= rotation;

      updateViewMatrix();
    }

    SetCursorPos((int32)m_originalMousePos.x, (int32)m_originalMousePos.y);
  }

  void 
  Camera::rotationAxis(const Vector2I& xy, const float& deltaTime) {
    Matrix4x4 rotation;

    float speedrot = 2.30f * deltaTime;

    if (xy.x > 0) {
      //rotation.rotationX(speedrot);
    }
    else if (xy.x < 0) {
      rotation.rotationX(-speedrot);
    }
    else if (xy.y > 0) {
      //rotation.rotationY(speedrot);
    }
    else if (xy.y < 0) {
      //rotation.rotationY(-speedrot);
    }

    m_view = rotation * m_view;

    updateViewMatrix();
  }

  /***************************************************************************/
  /**
  * Creates
  */
  /***************************************************************************/

  void
  Camera::createView() {
    m_front = m_cameraDesc.camLookAt - m_cameraDesc.camEye;
    m_front.normalize();

    m_right = m_cameraDesc.camUp.crossProduct(m_front);
    m_right.normalize();

    m_up = m_front.crossProduct(m_right);
    m_up.normalize();

    m_axis.calculateAxis(m_right, m_up, m_front);

    m_position.calculatePosition(m_cameraDesc.camEye);

    m_view = m_position * m_axis;
  }

  void
  Camera::createProjectionMatrix() {
    m_projection = m_projection.perspectiveFovLH(m_cameraDesc.camFoV,
                                                 (float)m_cameraDesc.camWidth,
                                                 (float)m_cameraDesc.camHeight,
                                                 m_cameraDesc.camNear,
                                                 m_cameraDesc.camFar);
  }

  /***************************************************************************/
  /**
  * Sets
  */
  /***************************************************************************/

  void
  Camera::setOriginalMousePos(float x, float y) {
    m_originalMousePos = { x, y };
  }

  void
  Camera::setLookAt(Vector3 lookAt) {
    m_cameraDesc.camLookAt = lookAt;
  }

  void
  Camera::setEye(Vector3 eye) {
    m_cameraDesc.camEye = eye;
  }

  void
  Camera::setUp(Vector3 up) {
    m_cameraDesc.camUp = up;
  }

  void
  Camera::setFar(float farCam) {
    m_cameraDesc.camFar = farCam;
  }

  void
  Camera::setNear(float nearCam) {
    m_cameraDesc.camNear = nearCam;
  }

  void
  Camera::setFoV(float fieldOfView) {
    m_cameraDesc.camFoV = fieldOfView;
  }

  void
  Camera::setHeight(float height) {
    m_cameraDesc.camHeight = height;
  }

  void
  Camera::setWidth(float width) {
    m_cameraDesc.camWidth = width;
  }

  /***************************************************************************/
  /**
  * Gets
  */
  /***************************************************************************/

  Matrix4x4
  Camera::getView() {
    return m_view;
  }

  Matrix4x4
  Camera::getProjection() {
    return m_projection;
  }

  Matrix4x4 
  Camera::getProjectionPerspective() {
    Matrix4x4 newProjection;
    newProjection = m_projection.perspectiveFovLH(m_cameraDesc.camFoV,
                                                 (float)m_cameraDesc.camWidth,
                                                 (float)m_cameraDesc.camHeight,
                                                 m_cameraDesc.camNear,
                                                 m_cameraDesc.camFar);
    return newProjection;
  }

  Vector2
  Camera::getOriginalMousePos() {
    return m_originalMousePos;
  }

  Vector3
  Camera::getCamEye() {
    return m_cameraDesc.camEye;
  }

  Vector3 
  Camera::getLookAt() {
    return m_cameraDesc.camLookAt;
  }

}