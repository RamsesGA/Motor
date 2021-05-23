#include "gaCamera.h"

namespace gaEngineSDK {
  
  void
  Camera::init(CameraDescriptor::E cameraDesc, bool isOGL) {
	  m_isOGL = isOGL;
	  
		m_cameraDesc.camUp     = cameraDesc.camUp;
		m_cameraDesc.camFar    = cameraDesc.camFar;
		m_cameraDesc.camFoV    = cameraDesc.camFoV;
		m_cameraDesc.camEye    = cameraDesc.camEye;
		m_cameraDesc.camNear   = cameraDesc.camNear;
	  m_cameraDesc.camWidth  = cameraDesc.camWidth;
		m_cameraDesc.camHeight = cameraDesc.camHeight;
		m_cameraDesc.camLookAt = cameraDesc.camLookAt;
	  
	  createView();
	  createProjectionMatrix();
	  updateViewMatrix();
  }
  
  void
  Camera::updateViewMatrix() {

    m_right = m_view.matrixData3(0);

    m_up = m_view.matrixData3(1);

    m_front = m_view.matrixData3(2);
	  
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

    float speedrot = 0.10f;

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

    float speedrot = 0.10f;

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

    float speedrot = 0.10f;

    if (sf::Keyboard::Z == param.key.code) {
      rotation.RotationY(speedrot);
    }
    else if (sf::Keyboard::C == param.key.code) {
      rotation.RotationY(-speedrot);
    }

    m_view = m_view * rotation;

    updateViewMatrix();
  }
  
  void 
  Camera::move(sf::Event param) {
  	if (sf::Keyboard::W == param.key.code) {
  		m_cameraDesc.camEye += m_front;
  	}
  	else if (sf::Keyboard::S == param.key.code) {
  		m_cameraDesc.camEye -= m_front;
  	}
  	else if (sf::Keyboard::A == param.key.code) {
  		m_cameraDesc.camEye -= m_right;
  	}
  	else if (sf::Keyboard::D == param.key.code) {
  		m_cameraDesc.camEye += m_right;
  	}
  	else if (sf::Keyboard::Q == param.key.code) {
  		m_cameraDesc.camEye += m_up;
  	}
  	else if (sf::Keyboard::E == param.key.code) {
  		m_cameraDesc.camEye -= m_up;
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

    firstPos.m_x = (float)Temp.x;
    secondPos.m_y = (float)Temp.y;

    if (firstPos.m_x < m_originalMousePos.m_x) {
      m_cameraDesc.camLookAt -= m_right * speedRot;
      m_cameraDesc.camUp = m_up;

      createView();
    }

    if (firstPos.m_x > m_originalMousePos.m_x) {
      m_cameraDesc.camLookAt += m_right * speedRot;
      m_cameraDesc.camUp = m_up;

      createView();
    }

    if ((secondPos.m_y < m_originalMousePos.m_y) && 
        (m_angule < m_maxAngule)) {
      m_angule += speedAngule;

      if (m_angule > m_maxAngule) {
        m_angule = m_maxAngule;
      }
      else {
        Pitch.RotationX(speedAngule * Math::PI / 180);
      }
    }

    if ((secondPos.m_y > m_originalMousePos.m_y) && 
        (m_angule > -m_maxAngule)) {
      m_angule -= speedAngule;

      if (m_angule < -m_maxAngule) {
        m_angule = -m_maxAngule;
      }
      else {
        Pitch.RotationX(-speedAngule * Math::PI / 180);
      }
    }

    SetCursorPos((int32)m_originalMousePos.m_x, (int32)m_originalMousePos.m_y);
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
  	//Usamos left hand
  	m_front = m_cameraDesc.camLookAt - m_cameraDesc.camEye;
    m_front.normalize();
  	
    m_right = m_cameraDesc.camUp.crossProduct(m_front);
  	m_right = { m_right.m_x, 0, m_right.m_z };
    m_right.normalize();

  	m_up = m_front.crossProduct(m_right);
    m_up.normalize();
  
    m_axis.calculateAxis(m_right, m_up, m_front);
  
    m_position.calculatePosition(m_cameraDesc.camEye);
  
    m_position = m_position * m_axis;
  	m_view = m_position;
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
  
  void Camera::setOriginalMousePos(float x, float y) {
  	m_originalMousePos = { x, y };
  }
  
  void Camera::setClickPressed(bool _bool) {
  	m_clickPressed = _bool;
  }
  
  /***************************************************************************/
  /**
  * Gets
  */
  /***************************************************************************/
  
  Matrix4x4
  Camera::getView() {
  	if (m_isOGL) {
  		return m_view.transpose();
  	}
  
  	return m_view;
  }
  
  Matrix4x4
  Camera::getProjection() {
  	if (m_isOGL) {
  		return m_projection;
  	}
  
  	return m_projection.transpose();
  }
  
  Vector2
  Camera::getOriginalMousePos() {
  	return m_originalMousePos;
  }
  
  bool Camera::getClickPressed() {
  	return m_clickPressed;
  }
}