//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   light.cpp
//  Description :   Code for Class CLight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes

// This Include
#include "light.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CLight constructor
*
* @author Christopher Howlett
*
*/
CLight::CLight()
:	m_fSpecularPower(0.0f)
,	m_bIsOn(false)
{

}
/**
*
* CLight destructor
*
* @author Christopher Howlett
*
*/
CLight::~CLight()
{

}
/**
*
* CLight Initialisation
*
* @author Christopher Howlett
* @param _vecDir Light Direction
* @param _vecColour Light Colour
* @param _fSpecularPower Light specular power
*
*/
void 
CLight::Initialise(TVector3& _rVecDir, TVector4& _rVecColour, float _fSpecularPower)
{
	m_vecDirection = _rVecDir;
	m_vecDirection.Normalise();
	m_vecColour = _rVecColour;
	m_fSpecularPower = _fSpecularPower;
}
/**
*
* CLight SetColour
*
* @author Christopher Howlett
* @param _rVecColour Colour to be set
*
*/
void 
CLight::SetColour(TVector4& _rVecColour)
{
	m_vecColour = _rVecColour;
}
/**
*
* CLight SetDirection
*
* @author Christopher Howlett
* @param _rVecDir Light Direction
*
*/
void 
CLight::SetDirection(TVector3& _rVecDir)
{
	m_vecDirection = _rVecDir;
	m_vecDirection.Normalise();
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
CLight::ProcessParent(float _fDeltaTime)
{
	if(m_bHasParent)
	{
		//Move object relative to parent
		m_vecPosition = m_pParent->GetPosition();
		m_vecDirection = m_pParent->GetForward();
	}
}
/**
*
* CLight GetColour
*
* @author Christopher Howlett
* @return Returns light Colour 
*
*/
TVector4& 
CLight::GetColour()
{
	return m_vecColour;
}
/**
*
* CLight GetDirection
*
* @author Christopher Howlett
* @return Returns light Direction
*
*/
TVector3& 
CLight::GetDirection()
{
	return m_vecDirection;
}
/**
*
* CLight GetSpecularPower
*
* @author Christopher Howlett
* @return Returns specular power
*
*/
float 
CLight::GetSpecularPower() const
{
	return m_fSpecularPower;
}
/**
*
* CLight IsOn
*
* @author Christopher Howlett
* @return Returns if light is turned on
*
*/
bool 
CLight::IsOn() const
{
	return m_bIsOn;
}
/**
*
* CLight ToggleIsOn
*
* @author Christopher Howlett
* @param _bIsOn Light is on
*
*/
void 
CLight::ToggleIsOn(bool _bIsOn)
{
	m_bIsOn = _bIsOn;
}