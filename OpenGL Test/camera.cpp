
// Library Includes
#include <glm/gtc/matrix_transform.hpp>

// Local Includes

// This Include
#include "camera.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* Camera constructor
*
* @author Christopher Howlett
*
*/
CCamera::CCamera()
:	m_fFOV(0.0f)
,	m_fNearClip(0.0f)
,	m_fFarClip(0.0f)
,	m_eCameraType(CAMERA_INVALID)
,	m_pInput(0)
,	m_fMovementSpeed(0.0f)
,	m_fRotationSpeed(0.0f)
,	m_fSpeedBoost(1.0f)
,	m_iScreenWidth(0)
,	m_iScreenHeight(0)
{
	m_matView = glm::mat4(1.0f);
	m_matProj = glm::mat4(1.0f);

	m_vecVelocity *= 0.0f;
}

/**
*
* Camera destructor
*
* @author Christopher Howlett
*
*/
CCamera::~CCamera()
{

}
	
/**
*
* Camera Initialise function
*
* @author Christopher Howlett
*
*/
bool 
CCamera::Initialise(	float _fFOV,
						float _fAspectRatio,
						float _fNearClip,
						float _fFarClip,
						float _fMovementSpeed,
						float _fRotationSpeed,
						int _iScreenWidth, 
						int _iScreenHeight,
						ECameraType _eCameraType)
{
	m_fAspectRatio = _fAspectRatio;	
	m_fFOV = _fFOV;
	m_fNearClip = _fNearClip;
	m_fFarClip = _fFarClip;
	m_eCameraType = _eCameraType;
	m_fMovementSpeed = _fMovementSpeed;
	m_fRotationSpeed = _fRotationSpeed;
	m_iScreenHeight = _iScreenHeight;
	m_iScreenWidth = _iScreenWidth;

	if(m_eCameraType == CAMERA_PERSPECTIVE)
	{
		//Calculate projection matrix
		m_matProj = glm::perspective(m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
	}
	else
	{
		//Calculate orthogonal matrix
		m_matProj = glm::ortho< int >(0, _iScreenWidth, 0, _iScreenHeight, static_cast<int>(m_fNearClip), static_cast<int>(m_fFarClip));
	}
	
	return 0;
}

/**
*
* Camera process function
*
* @author Christopher Howlett
* @param game delta tick
*
*/
void
CCamera::Process(float _fDeltaTick)
{
	//Recalculate view matrix
	m_matView = glm::lookAt(m_vecPosition, m_vecPosition + m_vecForward, m_vecUp);
	//NMatrix::Transformation(m_matWorld, m_vecPosition, m_vecScale, m_vecRotation);
}
/**
*
* Camera process input function
*
* @author Christopher Howlett
*
*/
void 
CCamera::ProcessInput(float _fDeltaTick)
{
	float fMouseSensitivity = 0.005f;
	//Process parented camera controls
	if(m_bHasParent)
	{
		if(m_pInput->bCtrl)
		{
			m_vecForward += m_vecRight * (m_pInput->fMouseX * fMouseSensitivity) * _fDeltaTick;
			//m_vecForward += m_vecUp * (m_pInput->fMouseY * fMouseSensitivity) * _fDeltaTick;
			glm::normalize( m_vecForward );
			glm::vec3 targetPosition = m_pParent->GetPosition() + ((m_pParent->GetForward() * m_vecParentOffset.z) + (m_vecUp * m_vecParentOffset.y));	
			m_vecPosition = targetPosition;
		}
		else
		{
			ProcessParent(_fDeltaTick);
		}
	}
	//Process free look camera controls
	else
	{
		if(m_pInput->bW)
		{
			m_vecVelocity += m_vecForward * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		}
		if(m_pInput->bS)
		{
			m_vecVelocity -= m_vecForward * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		}
		if(m_pInput->bA)
		{
			m_vecVelocity -= m_vecRight * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		}
		if(m_pInput->bD)
		{
			m_vecVelocity += m_vecRight * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		}
		if(m_pInput->bSpace)
		{
			m_vecVelocity += m_vecUp * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		}
		if(m_pInput->bCtrl)
		{
			m_vecVelocity -= m_vecUp * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		}
		if(m_pInput->bRightMouseDown)
		{
			//Mouse camera rotation
			m_vecForward += m_vecRight * (m_pInput->fMouseX * fMouseSensitivity) * _fDeltaTick;
			//m_vecForward += m_vecUp * (m_pInput->fMouseY * fMouseSensitivity * 0.5f) * _fDeltaTick;
		}
		if(m_pInput->bShift)
		{
			m_fSpeedBoost = 10.0f;
		}
		else
		{
			m_fSpeedBoost = 1.0f;
		}
		
		//Recalculate vectors
		m_vecUp = glm::vec3(0.0f, 1.0f, 0.0f);//NVector::Cross(m_vecForward, TVector3(1.0f, 0.0f, 0.0f));
		m_vecRight = glm::cross(m_vecForward, m_vecUp);
		glm::normalize( m_vecForward );
		glm::normalize( m_vecRight );
		glm::normalize( m_vecUp );
	
		m_vecPosition += m_vecVelocity * _fDeltaTick;
		m_vecVelocity *= 0.9f;
	}
}
/**
*
* Processes Parent functionality on this gameobject
*
* @author Christopher Howlett
* @param Game time elapsed
*
*/
void 
CCamera::ProcessParent(float _fDeltaTime)
{
	//Move object relative to parent
	m_vecForward += m_pParent->GetForward() * m_fRotationSpeed * _fDeltaTime;
	glm::normalize( m_vecForward );
	//m_vecForward = m_pParent->GetForward();
	m_vecUp = m_pParent->GetUp();
	m_vecRight = m_pParent->GetRight();

	glm::vec3 targetPosition = (m_pParent->GetPosition() + (m_pParent->GetForward() * m_vecParentOffset.z) + (m_vecUp * m_vecParentOffset.y));	
	m_vecPosition = targetPosition;
}
/**
*
* Camera Set input function
*
* @author Christopher Howlett
* @param Game input structure
*
*/
void 
CCamera::SetInput(TInputStruct* _pInput)
{
	m_pInput = _pInput;
}
/**
*
* Set clip planes
*
* @author Christopher Howlett
*
*/
void 
CCamera::SetClipPlanes(float _fNear, float _fFar)
{
	//Set members
	m_fNearClip = _fNear;
	m_fFarClip = _fFar;

	//Recalculate projection matrix
	m_matProj = glm::perspective( m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip );
}
/**
*
* GetNearPlane
*
* @author Christopher Howlett
*
*/
float 
CCamera::GetNearPlane() const
{
	return m_fNearClip;
}
/**
*
* GetFarPlane
*
* @author Christopher Howlett
* @return Far Plane
*
*/
float 
CCamera::GetFarPlane() const
{
	return m_fFarClip;
}
/**
*
* Get FOV
*
* @author Christopher Howlett
*
*/
void 
CCamera::SetFOV(float _fFOV)
{
	//Set members
	m_fFOV = _fFOV;
	
	//Recalculate projection matrix
	m_matProj = glm::perspective( m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip );
}

/**
*
* SetView3D - populate view matrix
*
* @author Christopher Howlett
*
*/
void 
CCamera::SetView3D(	glm::vec3& _rVecRight,
					glm::vec3& _rVecUp,
					glm::vec3& _rVecDir,
					glm::vec3& _rVecPos)
{
	//Populate view matrix
	m_matView = glm::mat4(1.0f);
	m_matView[0][0] = _rVecRight.x;
	m_matView[1][0] = _rVecRight.y;
	m_matView[2][0] = _rVecRight.z;
	m_matView[3][0] = 0.0f;
				  
	m_matView[0][1] = _rVecUp.x;
	m_matView[1][1] = _rVecUp.y;
	m_matView[2][1] = _rVecUp.z;
	m_matView[3][1] = 0.0f;
				  
	m_matView[0][2] = _rVecDir.x;
	m_matView[1][2] = _rVecDir.y;
	m_matView[2][2] = _rVecDir.z;
	m_matView[3][2] = 0.0f;
			  
	m_matView[0][3] = glm::dot(_rVecPos, _rVecRight) * -1;
	m_matView[1][3] = glm::dot(_rVecPos, _rVecUp) * -1;
	m_matView[2][3] = glm::dot(_rVecPos, _rVecDir) * -1;
	m_matView[3][3] = 1.0f;
}

/**
*
* SetViewLookAt - populate view matrix
*
* @author Christopher Howlett
*
*/
void 
CCamera::SetViewLookAt(	glm::vec3& _rVecPos,
							glm::vec3& _rVecLook,
							glm::vec3& _rVecUp )
{
	//Calculate right vector
	m_vecRight = glm::cross(m_vecForward, m_vecUp);
	glm::normalize( m_vecRight );
	
	//Calculate up vector
	glm::normalize( m_vecUp );
	
	//Call SetView3D
	SetView3D(m_vecRight, m_vecUp, m_vecForward, m_vecPosition);
}

/**
*
* Sets View Matrix
*
* @author Christopher Howlett
* @param View Matrix to be set
*
*/
void 
CCamera::SetViewMatrix(glm::mat4x4& _rMatView)
{
	//Store view matrix
	m_matView = _rMatView;
}
/**
*
* Sets Projection Matrix
*
* @author Christopher Howlett
* @param Projection Matrix to be set
*
*/
void 
CCamera::SetProjMatrix(glm::mat4x4& _rMatProj)
{
	//Store projection matrix
	m_matProj = _rMatProj;
}
/**
*
* Returns camera view matrix
*
* @author Christopher Howlett
* @return Camera view matrix
*
*/
glm::mat4x4& 
CCamera::GetViewMatrix()
{
	return m_matView;
}
/**
*
* Returns camera projection matrix
*
* @author Christopher Howlett
* @return Camera projection matrix
*
*/
glm::mat4x4& 
CCamera::GetProjectionMatrix()
{
	return m_matProj;
}
/**
*
* Returns camera world matrix
*
* @author Christopher Howlett
* @return Camera world matrix
*
*/
glm::mat4x4& 
CCamera::GetWorldMatrix()
{
	return m_matWorld;
}
/**
*
* Calculates 3D Mouse Coordinates
*
* @author Christopher Howlett
* @param _rVecMouse 2-Dimensional screen space coordinates
* @param _rRay containing resultant position and direction of mouse coordinate calculation
*
*/
void  
CCamera::Calculate3DMouseCoordinates(glm::vec2& _rVecMouse, TRay& _rRay)
{
	glm::vec3 h = glm::cross(m_vecForward, m_vecUp);
	glm::normalize( h );
	glm::vec3 v = glm::cross(h, m_vecForward);
	glm::normalize( v );
	
	//FOV to radians
	float fFieldOfView = glm::radians( m_fFOV );
	float fVerticalLength = tanf(fFieldOfView * 0.5f) * m_fNearClip;
	float fHorizontalLength = fVerticalLength * m_fAspectRatio;
	
	v *= fVerticalLength;
	h *= fHorizontalLength;
	
	//Scale mouse coordinates in range of 0 - 1
	_rVecMouse.x /= static_cast<float>(m_iScreenWidth * 0.5f);
	_rVecMouse.y /= static_cast<float>(m_iScreenHeight * 0.5f);
	
	//Calculate intersection with viewport near plane
	_rRay.vecPosition = m_vecPosition + (m_vecForward * m_fNearClip) + (h * _rVecMouse.x) + (v * _rVecMouse.y);
	
	//Calculate ray direction
	_rRay.vecDirection = _rRay.vecPosition - m_vecPosition;
	glm::normalize( _rRay.vecDirection );
	//_rRay.vecPosition += m_vecPosition;
}