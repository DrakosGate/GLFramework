//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   camera.cpp
//  Description :   Definition of camera class member functions
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include <math.h>

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
	NMatrix::Identity(m_matView);
	NMatrix::Identity(m_matProj);

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
		NMatrix::ProjectionPerspective(m_matProj, m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
	}
	else
	{
		//Calculate orthogonal matrix
		NMatrix::ProjectionOrthogonal(m_matProj, _iScreenWidth, _iScreenHeight, m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
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
	SetViewLookAt(m_vecPosition, m_vecForward, m_vecUp);
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
			m_vecForward.Normalise();
			TVector3 targetPosition = m_pParent->GetPosition() + ((m_pParent->GetForward() * m_vecParentOffset.fZ) + (m_vecUp * m_vecParentOffset.fY));	
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
		m_vecUp = TVector3(0.0f, 1.0f, 0.0f);//NVector::Cross(m_vecForward, TVector3(1.0f, 0.0f, 0.0f));
		m_vecRight = NVector::Cross(m_vecForward, m_vecUp);
		m_vecForward.Normalise();
		m_vecRight.Normalise();
		m_vecUp.Normalise();
	
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
	m_vecForward.Normalise();
	//m_vecForward = m_pParent->GetForward();
	m_vecUp = m_pParent->GetUp();
	m_vecRight = m_pParent->GetRight();

	TVector3 targetPosition = (m_pParent->GetPosition() + (m_pParent->GetForward() * m_vecParentOffset.fZ) + (m_vecUp * m_vecParentOffset.fY));	
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
	NMatrix::ProjectionPerspective(m_matProj, m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
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
	NMatrix::ProjectionPerspective(m_matProj, m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
}

/**
*
* SetView3D - populate view matrix
*
* @author Christopher Howlett
*
*/
void 
CCamera::SetView3D(	TVector3& _rVecRight,
					TVector3& _rVecUp,
					TVector3& _rVecDir,
					TVector3& _rVecPos)
{
	//Populate view matrix
	NMatrix::Identity(m_matView);
	m_matView.m[0] = _rVecRight.fX;
	m_matView.m[1] = _rVecRight.fY;
	m_matView.m[2] = _rVecRight.fZ;
	m_matView.m[3] = 0.0f;
				  
	m_matView.m[4] = _rVecUp.fX;
	m_matView.m[5] = _rVecUp.fY;
	m_matView.m[6] = _rVecUp.fZ;
	m_matView.m[7] = 0.0f;
				  
	m_matView.m[8] = _rVecDir.fX;
	m_matView.m[9] = _rVecDir.fY;
	m_matView.m[10] = _rVecDir.fZ;
	m_matView.m[11] = 0.0f;
			  
	m_matView.m[12] = NVector::Dot(_rVecPos, _rVecRight) * -1;
	m_matView.m[13] = NVector::Dot(_rVecPos, _rVecUp) * -1;
	m_matView.m[14] = NVector::Dot(_rVecPos, _rVecDir) * -1;
	m_matView.m[15] = 1.0f;
}

/**
*
* SetViewLookAt - populate view matrix
*
* @author Christopher Howlett
*
*/
void 
CCamera::SetViewLookAt(	TVector3& _rVecPos,
							TVector3& _rVecLook,
							TVector3& _rVecUp )
{
	//Calculate right vector
	m_vecRight = NVector::Cross(m_vecForward, m_vecUp);
	m_vecRight.Normalise();
	
	//Calculate up vector
	m_vecUp.Normalise();
	
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
CCamera::SetViewMatrix(TMatrix& _rMatView)
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
CCamera::SetProjMatrix(TMatrix& _rMatProj)
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
TMatrix& 
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
TMatrix& 
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
TMatrix& 
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
CCamera::Calculate3DMouseCoordinates(TVector2& _rVecMouse, TRay& _rRay)
{
	TVector3 h = NVector::Cross(m_vecForward, m_vecUp);
	h.Normalise();
	TVector3 v = NVector::Cross(h, m_vecForward);
	v.Normalise();
	
	//FOV to radians
	float fFieldOfView = m_fFOV * TO_RADIANS;
	float fVerticalLength = tanf(fFieldOfView * 0.5f) * m_fNearClip;
	float fHorizontalLength = fVerticalLength * m_fAspectRatio;
	
	v *= fVerticalLength;
	h *= fHorizontalLength;
	
	//Scale mouse coordinates in range of 0 - 1
	_rVecMouse.fX /= static_cast<float>(m_iScreenWidth * 0.5f);
	_rVecMouse.fY /= static_cast<float>(m_iScreenHeight * 0.5f);
	
	//Calculate intersection with viewport near plane
	_rRay.vecPosition = m_vecPosition + (m_vecForward * m_fNearClip) + (h * _rVecMouse.fX) + (v * _rVecMouse.fY);
	
	//Calculate ray direction
	_rRay.vecDirection = _rRay.vecPosition - m_vecPosition;
	_rRay.vecDirection.Normalise();
	//_rRay.vecPosition += m_vecPosition;
}