//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   lightmanager.h
//  Description :   Declaration of CLightManager class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __LIGHTMANAGER_H__
#define __LIGHTMANAGER_H__

// Library Includes
#include <vector>

// Local Includes
#include "chrismaths.h"

// Types
enum ELightType
{
	LIGHT_INVALID = -1,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_DIRECTIONAL,
	LIGHT_MAX
};

// Constants

// Prototypes
class CLight;
class CSpotLight;
class CPointLight;
class CDirectionalLight;

class CLightManager
{
// Member functions
public:
	CLightManager();
	~CLightManager();
	
	bool Initialise();
	void AddSpot(TVector3& _rVecPosition, TVector3& _rVecDirection, TVector4& _rVecColour, TVector3& _rAttenuation, float _fCutOff, float _fSpecularPower);
	void AddPoint(TVector3& _rVecPosition, TVector4& _rVecColour, TVector3& _rAttenuation, float _fSpecularPower);
	void AddDirectional(TVector3& _rVecDir, TVector4& _rVecColour, float _fSpecularPower);

	int GetLightCount(ELightType _eLightType) const;

	CSpotLight* GetSpot(int _iIndex);
	CPointLight* GetPoint(int _iIndex);
	CDirectionalLight* GetDirectional(int _iIndex);

protected:
	CLightManager(const CLightManager& _kr);
	CLightManager& operator= (const CLightManager& _rhs);

// Member variables
protected:
	std::vector<CSpotLight*> m_vSpotLights;
	std::vector<CPointLight*> m_vPointLights;
	std::vector<CDirectionalLight*> m_vDirectionalLights;
};

#endif //__LIGHTMANAGER_H__
