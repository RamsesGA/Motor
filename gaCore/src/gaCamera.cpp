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
  Camera::inputDetection(sf::Event param, const float& deltaTime) {
    if ((sf::Keyboard::Up == param.key.code) ||
        (sf::Keyboard::Down == param.key.code)) {
      pitchX(param);
    }
    if ((sf::Keyboard::Right == param.key.code) ||
        (sf::Keyboard::Left == param.key.code)) {
      rollZ(param);
    }
    if ((sf::Keyboard::Z == param.key.code) ||
        (sf::Keyboard::C == param.key.code)) {
      yawY(param);
    }
    else {
      move(param, deltaTime);
    }
  }

  void
  Camera::pitchX(sf::Event param) {
    Matrix4x4 rotation;

    float speedrot = 0.15f;

    if (sf::Keyboard::Up == param.key.code) {
      rotation.rotationX(speedrot);
    }
    else if (sf::Keyboard::Down == param.key.code) {
      rotation.rotationX(-speedrot);
    }

    m_view = rotation * m_view;

    updateViewMatrix();
  }

  void
  Camera::rollZ(sf::Event param) {
    Matrix4x4 rotation;

    float speedrot = 0.25f;

    if (sf::Keyboard::Right == param.key.code) {
      rotation.rotationZ(-speedrot);
    }
    else if (sf::Keyboard::Left == param.key.code) {
      rotation.rotationZ(speedrot);
    }

    m_view = rotation * m_view;

    updateViewMatrix();
  }

  void
  Camera::yawY(sf::Event param) {
    Matrix4x4 rotation;

    float speedrot = 0.15f;

    if (sf::Keyboard::Z == param.key.code) {
      rotation.rotationY(speedrot);
    }
    else if (sf::Keyboard::C == param.key.code) {
      rotation.rotationY(-speedrot);
    }

    m_view = m_view * rotation;

    updateViewMatrix();
  }

  void
  Camera::move(sf::Event param, const float& deltaTime) {
    float speedMove = 1550.0f * deltaTime;

    if (sf::Keyboard::W == param.key.code) {
      m_cameraDesc.camEye += m_front * speedMove;
    }
    else if (sf::Keyboard::S == param.key.code) {
      m_cameraDesc.camEye -= m_front * speedMove;
    }
    else if (sf::Keyboard::A == param.key.code) {
      m_cameraDesc.camEye -= m_right * speedMove;
    }
    else if (sf::Keyboard::D == param.key.code) {
      m_cameraDesc.camEye += m_right * speedMove;
    }
    else if (sf::Keyboard::Q == param.key.code) {
      m_cameraDesc.camEye += m_up * speedMove;
    }
    else if (sf::Keyboard::E == param.key.code) {
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
  Camera::setClickPressed(bool _bool) {
    m_clickPressed = _bool;
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

  Vector2
  Camera::getOriginalMousePos() {
    return m_originalMousePos;
  }

  bool
  Camera::getClickPressed() {
    return m_clickPressed;
  }

  Vector3
  Camera::getCamEye() {
    return m_cameraDesc.camEye;
  }
}