//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   pointlight.cpp
//  Description :   Code for Class pointlight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes

// This Include
#include "pointlight.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CPointLight constructor
*
* @author Christopher Howlett
*
*/
CPointLight::CPointLight()
{

}
/**
*
* CPointLight destructor
*
* @author Christopher Howlett
*
*/
CPointLight::~CPointLight()
{

}
/**
*
* CPointLight Initialisation
*
* @author Christopher Howlett
* @param _rVecPosition Light Position
* @param _vecColour Light Colour
* @param _rAttenuation Light Atten
* @param _fSpecularPower Light specular power
*
*/
bool 
CPointLight::Initialise(TVector3& _rVecPosition, TVector4& _rVecColour, TVector3& _rAttenuation, float _fSpecularPower)
{
	m_vecPosition = _rVecPosition;
	m_vecColour = _rVecColour;
	m_vecAttenuation = _rAttenuation;
	m_fSpecularPower = _fSpecularPower;

	return true;
}
/**
*
* CPointLight GetAttenuation
*
* @author Christopher Howlett
* @return Returns attenuation
*
*/
TVector3& 
CPointLight::GetAttenuation()
{
	return m_vecAttenuation;
}
/**
*
* CPointLight SetAttenuation
*
* @author Christopher Howlett
* @param _rAttenuation New attenuation
*
*/
void 
CPointLight::SetAttenuation(TVector3& _rAttenuation)
{
	m_vecAttenuation = _rAttenuation;
}