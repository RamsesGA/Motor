#include "gaCamera.h"

namespace gaEngineSDK {
  
  void
  Camera::init(CameraDescriptor::E cameraDesc,
  	           bool isOGL) {

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
      rotation =
      {
        1,	0,				0,					0,
        0,	cosf(speedrot),	-sinf(speedrot),	0,
        0,	sinf(speedrot),	cosf(speedrot),		0,
        0,	0,				0,					1
      };
    }
    else if (sf::Keyboard::Down == param.key.code) {
      rotation =
      {
        1,	0,					0,					0,
        0,	cosf(-speedrot),	-sinf(-speedrot),	0,
        0,	sinf(-speedrot),	cosf(-speedrot),	0,
        0,	0,					0,					1
      };
    }

    m_view = m_view * rotation;

    updateViewMatrix();
  }
  
  void 
  Camera::rollZ(sf::Event param) {
    Matrix4x4 rotation;

    float speedrot = 0.10f;

    if (sf::Keyboard::Right == param.key.code) {
      rotation =
      {
        cosf(speedrot),	-sinf(speedrot), 0,	0,
        sinf(speedrot),	cosf(speedrot),	 0,	0,
        0,				      0,					     1,	0,
        0,				      0,					     0,	1
      };
    }
    else if (sf::Keyboard::Left == param.key.code) {
      rotation =
      {
        cosf(-speedrot),	-sinf(-speedrot),	0, 0,
        sinf(-speedrot),	cosf(-speedrot),	0, 0,
        0,				        0,							  1, 0,
        0,				        0,							  0, 1
      };
    }

    m_view = m_view * rotation;

    updateViewMatrix();
  }
  
  void 
  Camera::yawY(sf::Event param) {
    Matrix4x4 rotation;

    float speedrot = 0.10f;

    if (sf::Keyboard::Z == param.key.code) {
      rotation =
      {
        cosf(speedrot),  0,	sinf(speedrot),	0,
        0,					     1,	0,				      0,
        -sinf(speedrot), 0,	cosf(speedrot),	0,
        0,					     0,	0,				      1
      };
    }
    else if (sf::Keyboard::C == param.key.code) {
      rotation =
      {
        cosf(-speedrot),  0,	sinf(-speedrot), 0,
        0,						    1,	0,					     0,
        -sinf(-speedrot), 0,	cosf(-speedrot), 0,
        0,						    0,	0,					     1
      };
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
  
  	m_axis =
  	{
  		m_right.m_x,	m_right.m_y,	m_right.m_z,	0,
  		m_up.m_x,		  m_up.m_y,		  m_up.m_z,		  0,
  		m_front.m_x,	m_front.m_y,	m_front.m_z,	0,
  		0,			      0,			      0,			      1
  	};
  
  	m_position =
  	{
  		1,	0,	0, -m_cameraDesc.camEye.m_x,
  		0,	1,	0, -m_cameraDesc.camEye.m_y,
  		0,	0,	1, -m_cameraDesc.camEye.m_z,
  		0,	0,	0,	1
  	};
  
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

    Matrix4x4 Yaw =
    {
      1,0,0,0,
      0,1,0,0,
      0,0,1,0,
      0,0,0,1
    };

    Matrix4x4 Pitch =
    {
      1,0,0,0,
      0,1,0,0,
      0,0,1,0,
      0,0,0,1
    };

    POINT Temp;
    GetCursorPos(&Temp);

    firstPos.m_x = Temp.x;
    secondPos.m_y = Temp.y;

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
        Pitch =
        {
          1,	0,									0,										0,
          0,	cosf(speedAngule * Math::PI / 180),	-sinf(speedAngule * Math::PI / 180),	0,
          0,	sinf(speedAngule * Math::PI / 180),	cosf(speedAngule * Math::PI / 180),		0,
          0,	0,									0,										1
        };
      }
    }

    if ((secondPos.m_y > m_originalMousePos.m_y) && 
        (m_angule > -m_maxAngule)) {
      m_angule -= speedAngule;

      if (m_angule < -m_maxAngule) {
        m_angule = -m_maxAngule;
      }
      else {
        Pitch =
        {
          1,	0,										0,										0,
          0,	cosf(-speedAngule * Math::PI / 180),	-sinf(-speedAngule * Math::PI / 180),	0,
          0,	sinf(-speedAngule * Math::PI / 180),	cosf(-speedAngule * Math::PI / 180),	0,
          0,	0,										0,										1
        };
      }

    }

    SetCursorPos(m_originalMousePos.m_x, m_originalMousePos.m_y);
    m_view = m_view * Yaw;

    updateViewMatrix();

    m_view = m_view * Pitch;

    updateViewMatrix();
  }
  
  ///
  /// C R E A T E´s
  /// 
  
  void 
  Camera::createView() {
  	//Usamos left hand
  	m_front = m_cameraDesc.camLookAt - m_cameraDesc.camEye;
  	//m_front = m_front.normalize();
    m_front.normalize();
  	
    m_right = m_cameraDesc.camUp.crossProduct(m_front);
  	m_right = { m_right.m_x, 0, m_right.m_z };
    //m_right = glm::normalize(m_right);
    m_right.normalize();

  	m_up = m_front.crossProduct(m_right);
    //m_up = glm::normalize(m_up);
    m_up.normalize();
  
  	m_axis =
  	{
  		m_right.m_x,	m_right.m_y,	m_right.m_z,	0,
  		m_up.m_x,		  m_up.m_y,		  m_up.m_z,		  0,
  		m_front.m_x,	m_front.m_y,	m_front.m_z,	0,
  		0,			      0,			      0,			      1
  	};
  
  	m_position =
  	{
  		1,	0,	0, -m_cameraDesc.camEye.m_x,
  		0,	1,	0, -m_cameraDesc.camEye.m_y,
  		0,	0,	1, -m_cameraDesc.camEye.m_z,
  		0,	0,	0,	1
  	};
  
  	/*m_axis =
  	{
  		m_right.m_x, m_up.m_x, m_front.m_x,    0,
  		m_right.m_y, m_up.m_y, m_front.m_y,    0,
  		m_right.m_z, m_up.m_z, m_front.m_z,    0,
  		0,			0,     0,			 1
  	};
  
  	m_position =
  	{
  		1,    0,    0,    0,
  		0,    1,    0,    0,
  		0,    0,    1,    0,
  		-m_cameraDesc.s_eye.m_x, -m_cameraDesc.s_eye.m_y, -m_cameraDesc.s_eye.m_z, 1
  	};*/
  
    m_position = m_position * m_axis;
  	m_view = m_position;
  }
  
  void
  Camera::createProjectionMatrix() {
  	m_projection = m_projection.perspectiveFovLH(m_cameraDesc.camFoV, 
                                                 m_cameraDesc.camWidth,
                                                 m_cameraDesc.camHeight, 
                                                 m_cameraDesc.camNear, 
                                                 m_cameraDesc.camFar);
  }
  
  ///
  /// S E T´s
  /// 
  
  void Camera::setOriginalMousePos(float x, float y) {
  	m_originalMousePos = { x, y };
  }
  
  void Camera::setClickPressed(bool _bool) {
  	m_clickPressed = _bool;
  }
  
  ///
  /// G E T´s
  /// 
  
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