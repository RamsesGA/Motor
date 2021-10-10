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

    m_camUp = m_view.getColumnMatrixInfo(1);

    m_front = m_view.getColumnMatrixInfo(2);

    m_camLookAt = m_camEye + m_front;
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
      m_camEye += m_front * speedMove;
    }
    else if (KEYBOARD::kS == input) {
      m_camEye -= m_front * speedMove;
    }
    else if (KEYBOARD::kA == input) {
      m_camEye -= m_right * speedMove;
    }
    else if (KEYBOARD::kD == input) {
      m_camEye += m_right * speedMove;
    }
    else if (KEYBOARD::kQ == input) {
      m_camEye += m_camUp * speedMove;
    }
    else if (KEYBOARD::kE == input) {
      m_camEye -= m_camUp * speedMove;
    }

    m_axis.calculateAxis(m_right, m_camUp, m_front);

    m_position.calculatePosition(m_camEye);
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
      m_camLookAt -= m_right * speedRot;
      m_camUp = m_camUp;
      createView();
      updateViewMatrix();
    }

    if (firstPos.x > m_originalMousePos.x) {
      m_camLookAt += m_right * speedRot;
      m_camUp = m_camUp;
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
    m_front = m_camLookAt - m_camEye;
    m_front.normalize();

    m_right = m_camUp.crossProduct(m_front);
    m_right.normalize();

    m_camUp = m_front.crossProduct(m_right);
    m_camUp.normalize();

    m_axis.calculateAxis(m_right, m_camUp, m_front);

    m_position.calculatePosition(m_camEye);

    m_view = m_position * m_axis;
  }

  void
  Camera::createProjectionMatrix() {
    m_projection = m_projection.perspectiveFovLH(m_camFoV,
                                                 (float)m_camWidth,
                                                 (float)m_camHeight,
                                                 m_camNear,
                                                 m_camFar);
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
    m_camLookAt = lookAt;
  }

  void
  Camera::setEye(Vector3 eye) {
    m_camEye = eye;
  }

  void
  Camera::setUp(Vector3 up) {
    m_camUp = up;
  }

  void
  Camera::setFar(float farCam) {
    m_camFar = farCam;
  }

  void
  Camera::setNear(float nearCam) {
    m_camNear = nearCam;
  }

  void
  Camera::setFoV(float fieldOfView) {
    m_camFoV = fieldOfView;
  }

  void
  Camera::setHeight(float height) {
    m_camHeight = height;
  }

  void
  Camera::setWidth(float width) {
    m_camWidth = width;
  }

  void
  Camera::setView(Matrix4x4 mView) {
    m_view = mView;
  }

  void
  Camera::setProjection(Matrix4x4 mProjection) {
    m_projection = mProjection;
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

  Vector2
  Camera::getOriginalMousePos() {
    return m_originalMousePos;
  }

  Vector3
  Camera::getCamEye() {
    return m_camEye;
  }

  Vector3 
  Camera::getLookAt() {
    return m_camLookAt;
  }

  Vector3 
  Camera::getCamUp() {
    return m_camUp;
  }

}