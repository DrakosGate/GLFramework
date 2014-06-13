//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   lightmanager.cpp
//  Description :   Definition of CLightManager class member functions
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "spotlight.h"
#include "pointlight.h"
#include "directionallight.h"

// This Include
#include "lightmanager.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CLightManager constructor
*
* @author Christopher Howlett
*
*/
CLightManager::CLightManager()
{
}

/**
*
* CLightManager destructor
*
* @author Christopher Howlett
*
*/
CLightManager::~CLightManager()
{
	for(unsigned int iLight = 0; iLight < m_vSpotLights.size(); ++iLight)
	{
		delete m_vSpotLights[iLight];
	}
	m_vSpotLights.clear();
	for(unsigned int iLight = 0; iLight < m_vPointLights.size(); ++iLight)
	{
		delete m_vPointLights[iLight];
	}
	m_vPointLights.clear();
	for(unsigned int iLight = 0; iLight < m_vDirectionalLights.size(); ++iLight)
	{
		delete m_vDirectionalLights[iLight];
	}
	m_vDirectionalLights.clear();
}
	
/**
*
* CLightManager Initialise function
*
* @author Christopher Howlett
*
*/
bool 
CLightManager::Initialise()
{
	return true;
}
void 
CLightManager::AddSpot(TVector3& _rVecPosition, TVector3& _rVecDirection, TVector4& _rVecColour, TVector3& _rAttenuation, float _fCutOff, float _fSpecularPower)
{
	CSpotLight* pNewSpot = new CSpotLight;
	pNewSpot->Initialise(_rVecPosition, _rVecDirection, _rVecColour, _rAttenuation, _fCutOff, _fSpecularPower);
	m_vSpotLights.push_back(pNewSpot);
}
void 
CLightManager::AddPoint(TVector3& _rVecPosition, TVector4& _rVecColour, TVector3& _rAttenuation, float _fSpecularPower)
{
	CPointLight* pNewPoint = new CPointLight;
	pNewPoint->Initialise(_rVecPosition, _rVecColour, _rAttenuation, _fSpecularPower);
	m_vPointLights.push_back(pNewPoint);
}
void 
CLightManager::AddDirectional(TVector3& _rVecDir, TVector4& _rVecColour, float _fSpecularPower)
{
	CDirectionalLight* pNewDirectional = new CDirectionalLight;
	pNewDirectional->Initialise(_rVecDir, _rVecColour, _fSpecularPower);
	m_vDirectionalLights.push_back(pNewDirectional);
}
int 
CLightManager::GetLightCount(ELightType _eLightType) const
{
	int iLightCount = 0;
	switch(_eLightType)
	{
	case LIGHT_SPOT:
		{
			iLightCount = m_vSpotLights.size();
		}
		break;
	case LIGHT_POINT:
		{
			iLightCount = m_vPointLights.size();
		}
		break;
	case LIGHT_DIRECTIONAL:
		{
			iLightCount = m_vDirectionalLights.size();
		}
		break;
	default:
		break;
	}
	return iLightCount;
}
CSpotLight* 
CLightManager::GetSpot(int _iIndex)
{
	return m_vSpotLights[_iIndex];
}
CPointLight* 
CLightManager::GetPoint(int _iIndex)
{
	return m_vPointLights[_iIndex];
}
CDirectionalLight* 
CLightManager::GetDirectional(int _iIndex)
{
	return m_vDirectionalLights[_iIndex];
}