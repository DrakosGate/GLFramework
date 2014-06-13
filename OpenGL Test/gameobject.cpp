//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   gameobject.cpp
//  Description :   Code for Class CGameObject
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes

// This Include
#include "gameobject.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CGameObject constructor
*
* @author Christopher Howlett
*
*/
CGameObject::CGameObject()
:	m_pParent(0)
,	m_bHasParent(false)
{
	NMatrix::Identity(m_matWorld);
	m_vecPosition *= 0.0f;
	m_vecScale *= 0.0f;
	m_vecRotation *= 0.0f;
	
	m_vecForward = TVector3(0.0f, 0.0f, 1.0f);
	m_vecRight = TVector3(1.0f, 0.0f, 0.0f);
	m_vecUp = TVector3(0.0f, 1.0f, 0.0f);

	m_vecParentOffset *= 0.0f;
}
/**
*
* CGameObject destructor
*
* @author Christopher Howlett
*
*/
CGameObject::~CGameObject()
{
	//Do nothing for this class
}
/**
*
* CGameObject Initialise
*
* @author Christopher Howlett
*
*/
bool 
CGameObject::Initialise()
{
	//Do nothing for this class
	return true;
}
/**
*
* CGameObject Draw
*
* @author Christopher Howlett
* @param _pRenderer OpenGL Renderer
* @param _pCamera Camera used for rendering
*
*/
void 
CGameObject::Draw(COpenGLRenderer* _pRenderer, CCamera* _pCamera)
{
	//Do nothing for this class
}
/**
*
* CGameObject Process
*
* @author Christopher Howlett
* @param _fDeltaTime Game Time elapsed
*
*/
void 
CGameObject::Process(float _fDeltaTime)
{
	NMatrix::Transformation(m_matWorld, m_vecPosition, m_vecScale, m_vecRotation);
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
CGameObject::ProcessParent(float _fDeltaTime)
{
	if(m_bHasParent)
	{
		//Move object relative to parent
		m_vecPosition = m_pParent->GetPosition();
		m_vecPosition += m_vecRight * m_vecParentOffset.fX;
		m_vecPosition += m_vecUp * m_vecParentOffset.fY;
		m_vecPosition += m_vecForward * m_vecParentOffset.fZ;
	}
}
/**
*
* Sets the parent object of this gameobject - Pass NULL or 0 to remove parent object
*
* @author Christopher Howlett
* @param Parent gameobject assigned
*
*/
void 
CGameObject::SetParent(CGameObject* _pParent, TVector3* _pTargetOffset)
{
	if(_pParent)
	{
		m_bHasParent = true;
		m_pParent = _pParent;
		m_vecParentOffset = *_pTargetOffset;
	}
	else
	{
		m_bHasParent = false;
		m_pParent = 0;
		m_vecParentOffset *= 0.0f;
	}
}
/**
*
* Sets CGameObject position
*
* @author Christopher Howlett
* @param position of gameobject
*
*/
void 
CGameObject::SetPosition(TVector3& _rVecPosition)
{
	m_vecPosition = _rVecPosition;
}
/**
*
* Returns CGameObject position
*
* @author Christopher Howlett
* @return Gameobject position
*
*/
TVector3& 
CGameObject::GetPosition()
{
	return m_vecPosition;
}
/**
*
* Sets GameObject rotation
*
* @author Christopher Howlett
* @param Gameobject rotation
*
*/
void 
CGameObject::SetRotation(TVector3& _rRotation)
{
	m_vecRotation = _rRotation;
}
/**
*
* Returns GameObject rotation
*
* @author Christopher Howlett
* @return Gameobject rotation
*
*/
TVector3& 
CGameObject::GetRotation()
{
	return m_vecRotation;
}
/**
*
* Sets GameObject scale
*
* @author Christopher Howlett
* @param Gameobject scale
*
*/
void 
CGameObject::SetScale(TVector3& _rScale)
{
	m_vecScale = _rScale;
}
/**
*
* Returns GameObject scale
*
* @author Christopher Howlett
* @return Gameobject scale
*
*/
TVector3& 
CGameObject::GetScale()
{
	return m_vecScale;
}
/**
*
* Sets GameObject Forward
*
* @author Christopher Howlett
* @param Gameobject Forward
*
*/
void 
CGameObject::SetForward(TVector3& _rForward)
{
	m_vecForward = _rForward;
	m_vecForward.Normalise();
}
/**
*
* Returns GameObject Forward
*
* @author Christopher Howlett
* @return Gameobject Forward
*
*/
TVector3& 
CGameObject::GetForward()
{
	m_vecForward.Normalise();
	return m_vecForward;
}
/**
*
* Sets GameObject Right
*
* @author Christopher Howlett
* @param Gameobject Forward
*
*/
void 
CGameObject::SetRight(TVector3& _rRight)
{
	m_vecRight = _rRight;
	m_vecRight.Normalise();
}
/**
*
* Returns GameObject Right
*
* @author Christopher Howlett
* @return Gameobject Right
*
*/
TVector3& 
CGameObject::GetRight()
{
	m_vecRight.Normalise();
	return m_vecRight;
}
/**
*
* Sets GameObject Up
*
* @author Christopher Howlett
* @param Gameobject Up
*
*/
void 
CGameObject::SetUp(TVector3& _rUp)
{
	m_vecUp = _rUp;
	m_vecUp.Normalise();
}
/**
*
* Returns GameObject Up
*
* @author Christopher Howlett
* @return Gameobject Up
*
*/
TVector3& 
CGameObject::GetUp()
{
	m_vecUp.Normalise();
	return m_vecUp;
}