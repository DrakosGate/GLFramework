//  File Name   :   helicopter.cpp
//  Description :   Definition of helicopter class member functions
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <math.h>

// Local Includes
#include "shader.h"
#include "model.h"

// This Include
#include "helicopter.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* Helicopter constructor
*
* @author Christopher Howlett
*
*/
CHelicopter::CHelicopter()
:	m_fRotorRotation(0.0f)
,	m_fMovementSpeed(10.0f)
,	m_fRotationSpeed(1.0f)
,	m_fSpeedBoost(1.0f)
,	m_fLift(0.0f)
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

	m_vecRotorOffset = TVector3(0.0f, 0.0f, 0.0f);
}

/**
*
* Helicopter destructor
*
* @author Christopher Howlett
*
*/
CHelicopter::~CHelicopter()
{

}
	
/**
*
* Helicopter Initialise function
*
* @author Christopher Howlett
*
*/
bool 
CHelicopter::Initialise(CModel* _pChassisModel, CModel* _pRotorModel, TInputStruct* _pInput)
{
	m_pChassis = _pChassisModel;
	m_pRotor = _pRotorModel;
	m_pInput = _pInput;

	m_fMass = 10.0f;
	m_fGravity = 20.0f;

	return true;
}

/**
*
* Helicopter process function
*
* @author Christopher Howlett
* @param game delta tick
*
*/
void
CHelicopter::Process(float _fDeltaTick)
{
	//m_vecForward += (m_vecPosition * -1.0f) * _fDeltaTick;
	//m_vecPosition += m_vecForward * _fDeltaTick;
	m_vecRotation.fX = atan2f(m_vecUp.fZ, m_vecUp.fY);
	m_vecRotation.fY = atan2f(m_vecForward.fZ, m_vecForward.fX);
	m_vecRotation.fZ = atan2f(m_vecUp.fX, m_vecUp.fY);
	

	SetPosition(m_vecPosition);
	
	m_fRotorRotation += m_fLift * _fDeltaTick;
	m_pChassis->Process(_fDeltaTick);
	m_pRotor->Process(_fDeltaTick);
	m_pChassis->SetRotation(m_vecRotation);
	m_pRotor->SetRotation(TVector3(m_vecRotation.fX, m_fRotorRotation, m_vecRotation.fZ));
}
/**
*
* Helicopter Draw function
*
* @author Christopher Howlett
* @param Renderer
*
*/
void 
CHelicopter::Draw(COpenGLRenderer* _pRenderer, CCamera* _pCamera, CShader* _pShader)
{
	_pShader->SetShaderMatrix(_pRenderer, "worldMatrix", m_pChassis->GetWorldMatrix()->m);
	m_pChassis->Draw(_pRenderer, _pCamera);
	_pShader->SetShaderMatrix(_pRenderer, "worldMatrix", m_pRotor->GetWorldMatrix()->m);
	m_pRotor->Draw(_pRenderer, _pCamera);
}

/**
*
* Helicopter process input function
*
* @author Christopher Howlett
*
*/
void 
CHelicopter::ProcessInput(float _fDeltaTick)
{
	float fMouseSensitivity = 0.005f;
	if(m_pInput->bW)
	{
		//m_vecVelocity += m_vecForward * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		m_vecUp -= m_vecForward * m_fRotationSpeed * m_fSpeedBoost * _fDeltaTick;
	}
	if(m_pInput->bS)
	{
		//m_vecVelocity -= m_vecForward * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		m_vecUp += m_vecForward * m_fRotationSpeed * m_fSpeedBoost * _fDeltaTick;
	}
	if(m_pInput->bA)
	{
		//m_vecVelocity -= m_vecRight * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		m_vecUp += m_vecRight * m_fRotationSpeed * m_fSpeedBoost * _fDeltaTick;
	}
	if(m_pInput->bD)
	{
		//m_vecVelocity += m_vecRight * m_fMovementSpeed * m_fSpeedBoost * _fDeltaTick;
		m_vecUp -= m_vecRight * m_fRotationSpeed * m_fSpeedBoost * _fDeltaTick;
	}
	if(m_pInput->bSpace)
	{
		m_fLift += m_fMovementSpeed * _fDeltaTick;
	}
	if(m_pInput->bCtrl)
	{
		m_fLift -= m_fMovementSpeed * _fDeltaTick;
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
	//Attempt to maintain vertical 
	
	m_vecVelocity += m_vecUp * (m_fLift - m_fMass - m_fGravity) * _fDeltaTick;

	m_vecForward.Normalise();
	m_vecRight.Normalise();
	m_vecUp.Normalise();
	m_vecUp += TVector3(0.0f, 1.0f, 0.0f) * _fDeltaTick;
	m_vecRight = NVector::Cross(m_vecForward, m_vecUp);
	m_vecForward = NVector::Cross(m_vecUp, m_vecRight);

	m_vecPosition += m_vecVelocity * _fDeltaTick;
	m_vecVelocity *= 0.98f;
	m_fLift *= 0.9998f;

	
	if(m_vecPosition.fY < 0.1f)
	{
		m_vecPosition.fY = 0.1f;
	}
}
/**
*
* Sets Helicopter position
*
* @author Christopher Howlett
* @param position of Helicopter
*
*/
void 
CHelicopter::SetPosition(TVector3& _rVecPosition)
{
	m_vecPosition = _rVecPosition;
	m_pChassis->SetPosition(m_vecPosition);
	m_pRotor->SetPosition(m_vecPosition);
}