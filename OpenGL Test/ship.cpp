//  File Name   :   ship.cpp
//  Description :   Definition of ship class member functions
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <math.h>

// Local Includes
#include "shader.h"
#include "model.h"
#include "openglrenderer.h"

// This Include
#include "ship.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CShip constructor
*
* @author Christopher Howlett
*
*/
CShip::CShip()
:	m_fMovementSpeed(10.0f)
,	m_fRotationSpeed(10.0f)
,	m_fSpeedBoost(1.0f)
{
	m_vecVelocity *= 0.0f;
	m_vecPosition.fX = 0.0f;
	m_vecPosition.fY = 2.0f;
	m_vecPosition.fZ = -5.0f;

	m_vecForward.fX = 0.0f;
	m_vecForward.fY = 0.0f;
	m_vecForward.fZ = 1.0f;

	m_vecRight.fX = 1.0f;
	m_vecRight.fY = 0.0f;
	m_vecRight.fZ = 0.0f;

	m_vecUp.fX = 0.0f;
	m_vecUp.fY = 1.0f;
	m_vecUp.fZ = 0.0f;
}

/**
*
* CShip destructor
*
* @author Christopher Howlett
*
*/
CShip::~CShip()
{

}
	
/**
*
* CShip Initialise function
*
* @author Christopher Howlett
*
*/
bool 
CShip::Initialise(CModel* _pChassisModel, TInputStruct* _pInput)
{
	m_pChassis = _pChassisModel;
	m_pInput = _pInput;

	m_fMass = 10.0f;
	m_fGravity = 20.0f;

	return true;
}

/**
*
* CShip process function
*
* @author Christopher Howlett
* @param game delta tick
*
*/
void
CShip::Process(float _fDeltaTick)
{
	m_vecRotation.fY = atan2(-m_vecForward.fX, m_vecForward.fZ);
	//m_vecRotation.fZ = atan2(m_vecUp.fZ, m_vecUp.fY);
	
	m_pChassis->Process(_fDeltaTick);
	m_pChassis->SetPosition(m_vecPosition);
	m_pChassis->SetRotation(m_vecRotation);
}
/**
*
* CShip Draw function
*
* @author Christopher Howlett
* @param Renderer
*
*/
void 
CShip::Draw(COpenGLRenderer* _pRenderer, CCamera* _pCamera, CShader* _pShader)
{
	_pShader->SetShaderMatrix(_pRenderer, "worldMatrix", m_pChassis->GetWorldMatrix()->m);
	m_pChassis->Draw(_pRenderer, _pCamera);
}

/**
*
* CShip process input function
*
* @author Christopher Howlett
*
*/
void 
CShip::ProcessInput(float _fDeltaTick)
{
	//Ensure alt is not pressed
	if(m_pInput->bCtrl == false)
	{
		float fMouseSensitivity = 0.005f;
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
		if(m_pInput->bUp)
		{
			m_vecVelocity += m_vecUp * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		}
		if(m_pInput->bDown)
		{
			m_vecVelocity -= m_vecUp * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		}
		if(m_pInput->bRightMouseDown)
		{
			//Mouse camera rotation
			//m_vecForward += m_vecRight * (m_pInput->vecMouseDir.fX * m_fRotationSpeed * fMouseSensitivity) * _fDeltaTick;
			m_vecForward += m_vecRight * (m_pInput->fMouseX * fMouseSensitivity) * _fDeltaTick;
			//m_vecUp += m_vecRight * m_fRotationSpeed * _fDeltaTick;
		}
		if(m_pInput->bShift)
		{
			m_fSpeedBoost = 10.0f;
		}
		else
		{
			m_fSpeedBoost = 1.0f;
		}
	}
	
	m_vecForward.Normalise();
	m_vecRight.Normalise();
	m_vecUp.Normalise();
	//m_vecUp += TVector3(0.0f, 1.0f, 0.0f) * _fDeltaTick;
	m_vecRight = NVector::Cross(m_vecForward, TVector3(0.0f, 1.0f, 0.0f));
	m_vecUp = NVector::Cross(m_vecRight, m_vecForward);

	m_vecPosition += m_vecVelocity * _fDeltaTick;
	m_vecVelocity *= 0.98f;

	
	if(m_vecPosition.fY < 0.1f)
	{
		m_vecPosition.fY = 0.1f;
	}
}
/**
*
* Sets Ship position
*
* @author Christopher Howlett
* @param position of Ship
*
*/
void 
CShip::SetPosition(TVector3& _rVecPosition)
{
	m_vecPosition = _rVecPosition;
	m_pChassis->SetPosition(m_vecPosition);
}