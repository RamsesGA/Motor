#include "gaCamera.h"

namespace gaEngineSDK {
  
  void
  Camera::init() {
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
  Camera::inputDetection(sf::Event param) {
	  if ((sf::Keyboard::Up == param.key.code)  || 
        (sf::Keyboard::Down == param.key.code)) {
	  	pitchX(param);
	  }
	  if ((sf::Keyboard::Right == param.key.code) ||
        (sf::Keyboard::Left == param.key.code)) {
	  	rollZ(param);
	  }
	  if ((sf::Keyboard::Z == param.key.code)||
			  (sf::Keyboard::C == param.key.code)) {
	  	yawY(param);
	  }
	  else {
	  	move(param);
	  }
  }
  
  void
  Camera::pitchX(sf::Event param) {
    Matrix4x4 rotation;

    float speedrot = 1.35f;

    if (sf::Keyboard::Up == param.key.code) {
      rotation.RotationX(speedrot);
    }
    else if (sf::Keyboard::Down == param.key.code) {
      rotation.RotationX(-speedrot);
    }

    m_view = m_view * rotation;

    updateViewMatrix();
  }
  
  void 
  Camera::rollZ(sf::Event param) {
    Matrix4x4 rotation;

    float speedrot = 1.35f;

    if (sf::Keyboard::Right == param.key.code) {
      rotation.RotationZ(-speedrot);
    }
    else if (sf::Keyboard::Left == param.key.code) {
      rotation.RotationZ(speedrot);
    }

    m_view = m_view * rotation;

    updateViewMatrix();
  }
  
  void 
  Camera::yawY(sf::Event param) {
    Matrix4x4 rotation;

    float speedrot = 1.35f;

    if (sf::Keyboard::Z == param.key.code) {
      rotation.RotationY(speedrot);
    }
    else if (sf::Keyboard::C == param.key.code) {
      rotation.RotationY(-speedrot);
    }

    m_view = m_view * rotation;

    updateViewMatrix();
  }
  
  //Se va a mover 15 frames por segundo, necesito pasarle el Delta time
  //y que esta se vuelva su velocidad: ej: 10 * deltaTime
  void 
  Camera::move(sf::Event param) {
    float speedMove = 10.0f;

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
  Camera::mouseRotation() {
    Vector2 firstPos;
    Vector2 secondPos;

    float speedRot = 0.010f;
    float speedAngule = 0.5f;

    Matrix4x4 Yaw;
    Yaw.identity();

    Matrix4x4 Pitch;
    Pitch.identity();

    POINT Temp;
    GetCursorPos(&Temp);

    firstPos.x = (float)Temp.x;
    secondPos.y = (float)Temp.y;

    if (firstPos.x < m_originalMousePos.x) {
      m_cameraDesc.camLookAt -= m_right * speedRot;
      m_cameraDesc.camUp = m_up;

      createView();
    }

    if (firstPos.x > m_originalMousePos.x) {
      m_cameraDesc.camLookAt += m_right * speedRot;
      m_cameraDesc.camUp = m_up;

      createView();
    }

    if ((secondPos.y < m_originalMousePos.y) && 
        (m_angule < m_maxAngule)) {
      m_angule += speedAngule;

      if (m_angule > m_maxAngule) {
        m_angule = m_maxAngule;
      }
      else {
        Pitch.RotationX(speedAngule * Math::PI / 180);
      }
    }

    if ((secondPos.y > m_originalMousePos.y) && 
        (m_angule > -m_maxAngule)) {
      m_angule -= speedAngule;

      if (m_angule < -m_maxAngule) {
        m_angule = -m_maxAngule;
      }
      else {
        Pitch.RotationX(-speedAngule * Math::PI / 180);
      }
    }

    SetCursorPos((int32)m_originalMousePos.x, (int32)m_originalMousePos.y);
    m_view = m_view * Yaw;

    updateViewMatrix();

    m_view = m_view * Pitch;

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
    //m_position = m_position * m_axis;

    m_view = m_axis * m_position;
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
  
}