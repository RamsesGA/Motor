#include "gaCamera.h"

namespace gaEngineSDK {
  
  void
  Camera::init(CameraDescriptor cameraDesc,
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
	  m_right.m_x = m_view[0].x;
	  m_right.m_y = m_view[0].y;
	  m_right.m_z = m_view[0].z;
	  
	  m_up.m_x = m_view[1].x;
		m_up.m_y = m_view[1].y;
		m_up.m_z = m_view[1].z;
	  
	  m_front.m_x = m_view[2].x;
		m_front.m_y = m_view[2].y;
		m_front.m_z = m_view[2].z;
	  
	  /*m_right = { m_view[0][0], m_view[1][0], m_view[2][0] };
	  m_up =	{	m_view[0][1], m_view[1][1], m_view[2][1] };
	  m_front = { m_view[0][2], m_view[1][2], m_view[2][2] };*/
	  
	  m_cameraDesc.camLookAt = m_cameraDesc.camEye + m_front;
  }
  
  void
  Camera::inputDetection(WPARAM param) {
	  if ((VK_UP == param)  || (param == VK_DOWN)) {
	  	pitchX(param);
	  }
	  if ((VK_RIGHT == param) || (VK_LEFT == param)) {
	  	rollZ(param);
	  }
	  if (('z' == param) || ('Z' == param) || 
			  ('c' == param )|| ('C' == param)) {
	  	yawY(param);
	  }
	  else {
	  	move(param);
	  }
  }
  
  void
  Camera::pitchX(WPARAM _param) {

	glm::mat4x4 rotation;
	float speedrot = 0.10f;

	if (VK_UP == _param) {

		rotation =
		{
			1,	0,				0,					0,
			0,	cosf(speedrot),	-sinf(speedrot),	0,
			0,	sinf(speedrot),	cosf(speedrot),		0,
			0,	0,				0,					1
		};
	}
	else if (VK_DOWN == _param) {

		rotation =
		{
			1,	0,					0,					0,
			0,	cosf(-speedrot),	-sinf(-speedrot),	0,
			0,	sinf(-speedrot),	cosf(-speedrot),	0,
			0,	0,					0,					1
		};
	}

	m_view *= rotation;

	UpdateViewMatrix();
}
  
  void 
  Camera::rollZ(WPARAM _param) {

	glm::mat4x4 rotation;
	float speedrot = 0.10f;

	if (VK_RIGHT == _param) {

		rotation =
		{
			cosf(speedrot),	-sinf(speedrot),	0,	0,
			sinf(speedrot),	cosf(speedrot),		0,	0,
			0,				0,					1,	0,
			0,				0,					0,	1
		};
	}
	else if (VK_LEFT == _param) {

		rotation =
		{
			cosf(-speedrot),	-sinf(-speedrot),		0,		0,
			sinf(-speedrot),	cosf(-speedrot),		0,		0,
			0,				0,							1,		0,
			0,				0,							0,		1
		};
	}

	m_view *= rotation;

	UpdateViewMatrix();
}
  
  void 
  Camera::yawY(WPARAM _param) {

	glm::mat4x4 rotation;
	float speedrot = 0.10f;

	if ('z' == _param ||
		'Z' == _param) {

		rotation =
		{
			cosf(speedrot),		0,	sinf(speedrot),	0,
			0,					1,	0,				0,
			-sinf(speedrot),	0,	cosf(speedrot),	0,
			0,					0,	0,				1
		};
	}
	else if ('c' == _param ||
		'C' == _param) {

		rotation =
		{
			cosf(-speedrot),		0,	sinf(-speedrot),	0,
			0,						1,	0,					0,
			-sinf(-speedrot),		0,	cosf(-speedrot),	0,
			0,						0,	0,					1
		};
	}

	m_view *= rotation;

	UpdateViewMatrix();
}
  
  void 
  Camera::move(WPARAM _param) {

	if (_param == 'w' ||
		'W' == _param) {

		m_cameraDesc.s_eye += m_front;
	}
	else if (_param == 's' ||
		'S' == _param) {

		m_cameraDesc.s_eye -= m_front;
	}
	else if (_param == 'a' ||
		'A' == _param) {

		m_cameraDesc.s_eye -= m_right;
	}
	else if (_param == 'd' ||
		'D' == _param) {

		m_cameraDesc.s_eye += m_right;
	}
	else if (_param == 'q' ||
		'Q' == _param) {

		m_cameraDesc.s_eye += m_up;
	}
	else if (_param == 'e' ||
		'E' == _param) {

		m_cameraDesc.s_eye -= m_up;
	}

	m_axis =
	{
		m_right.x,	m_right.y,	m_right.z,	0,
		m_up.x,		m_up.y,		m_up.z,		0,
		m_front.x,	m_front.y,	m_front.z,	0,
		0,			0,			0,			1
	};

	m_position =
	{
		1,	0,	0, -m_cameraDesc.s_eye.x,
		0,	1,	0, -m_cameraDesc.s_eye.y,
		0,	0,	1, -m_cameraDesc.s_eye.z,
		0,	0,	0,	1
	};


	m_position *= m_axis;

	m_view = m_position;

	UpdateViewMatrix();
}
  
  void 
  Camera::mouseRotation() {

	glm::vec2 firstPos;
	glm::vec2 secondPos;

	float speedRot = 0.010f;
	float speedAngule = 0.5f;

	glm::mat4x4 Yaw =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	glm::mat4x4 Pitch =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	POINT Temp;
	GetCursorPos(&Temp);

	firstPos.x = Temp.x;
	secondPos.y = Temp.y;

	if (firstPos.x < m_originalMousePos.x) {

		m_cameraDesc.s_lookAt -= m_right * speedRot;
		m_cameraDesc.s_up = m_up;

		CreateView();
	}

	if (firstPos.x > m_originalMousePos.x) {

		m_cameraDesc.s_lookAt += m_right * speedRot;
		m_cameraDesc.s_up = m_up;
		CreateView();
	}

	if (secondPos.y < m_originalMousePos.y && m_angule < m_maxAngule) {

		m_angule += speedAngule;

		if (m_angule > m_maxAngule) {

			m_angule = m_maxAngule;
		}
		else {

			Pitch =
			{
				1,	0,									0,										0,
				0,	cosf(speedAngule * 3.141516 / 180),	-sinf(speedAngule * 3.141516 / 180),	0,
				0,	sinf(speedAngule * 3.141516 / 180),	cosf(speedAngule * 3.141516 / 180),		0,
				0,	0,									0,										1
			};
		}

	}

	if (secondPos.y > m_originalMousePos.y && m_angule > -m_maxAngule) {

		m_angule -= speedAngule;

		if (m_angule < -m_maxAngule) {

			m_angule = -m_maxAngule;
		}
		else {

			Pitch =
			{
				1,	0,										0,										0,
				0,	cosf(-speedAngule * 3.141516 / 180),	-sinf(-speedAngule * 3.141516 / 180),	0,
				0,	sinf(-speedAngule * 3.141516 / 180),	cosf(-speedAngule * 3.141516 / 180),	0,
				0,	0,										0,										1
			};
		}

	}

	SetCursorPos(m_originalMousePos.x, m_originalMousePos.y);
	m_view *= Yaw;
	UpdateViewMatrix();

	m_view *= Pitch;
	UpdateViewMatrix();
}
  
  ///
  /// C R E A T E´s
  /// 
  
  void 
  Camera::createView() {

	//Usamos left hand
	m_front = m_cameraDesc.s_lookAt - m_cameraDesc.s_eye;
	m_front = glm::normalize(m_front);

	m_right = glm::cross(m_cameraDesc.s_up, m_front);
	m_right = { m_right.x, 0, m_right.z };
	m_right = glm::normalize(m_right);

	m_up = glm::cross(m_front, m_right);
	m_up = glm::normalize(m_up);

	m_axis =
	{
		m_right.x,	m_right.y,	m_right.z,	0,
		m_up.x,		m_up.y,		m_up.z,		0,
		m_front.x,	m_front.y,	m_front.z,	0,
		0,			0,			0,			1
	};

	m_position =
	{
		1,	0,	0, -m_cameraDesc.s_eye.x,
		0,	1,	0, -m_cameraDesc.s_eye.y,
		0,	0,	1, -m_cameraDesc.s_eye.z,
		0,	0,	0,	1
	};

	/*m_axis =
	{
		m_right.x, m_up.x, m_front.x,    0,
		m_right.y, m_up.y, m_front.y,    0,
		m_right.z, m_up.z, m_front.z,    0,
		0,			0,     0,			 1
	};

	m_position =
	{
		1,    0,    0,    0,
		0,    1,    0,    0,
		0,    0,    1,    0,
		-m_cameraDesc.s_eye.x, -m_cameraDesc.s_eye.y, -m_cameraDesc.s_eye.z, 1
	};*/

	//m_view = m_axis * m_position;
	m_position *= m_axis;
	m_view = m_position;
}
  
  void
  Camera::createProjectionMatrix() {

	m_projection = glm::perspectiveFovLH(m_cameraDesc.s_foV, m_cameraDesc.s_width,
		m_cameraDesc.s_height, m_cameraDesc.s_near, m_cameraDesc.s_far);
}
  
  ///
  /// S E T´s
  /// 
  
  void Camera::setOriginalMousePos(float _x, float _y) {

	m_originalMousePos = { _x, _y };
}
  
  void Camera::setClickPressed(bool _bool) {

	m_clickPressed = _bool;
}
  
  ///
  /// G E T´s
  /// 
  
  glm::mat4x4 Camera::getView() {

	if (m_isOGL) {

		return glm::transpose(m_view);
	}

	return m_view;
}
  
  glm::mat4x4 Camera::getProjection() {

	if (m_isOGL) {

		return m_projection;
	}

	return glm::transpose(m_projection);
}
  
  glm::vec2 Camera::getOriginalMousePos() {

	return m_originalMousePos;
}
  
  bool Camera::getClickPressed() {

	return m_clickPressed;
}
  
}