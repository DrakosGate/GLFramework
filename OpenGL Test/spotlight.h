//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   CSpotLight.h
//  Description :   Class of CSpotLight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __SPOTLIGHT_H__
#define __SPOTLIGHT_H__

// Library Includes

// Local Includes
#include "light.h"

// Types

// Constants

// Prototypes

class CSpotLight : public CLight
{
public:
	CSpotLight();
	virtual ~CSpotLight();

	virtual bool Initialise(TVector3& _rVecPosition, TVector3& _rVecDirection, TVector4& _rVecColour, TVector3& _rAttenuation, float _fCutOff, float _fSpecularPower);
	
	TVector3& GetAttenuation();
	void SetAttenuation(TVector3& _rAttenuation);
	float GetCutoff() const;
	void SetCutoff(float _fCutoff);

private:
	CSpotLight(const CSpotLight& _krInstanceToCopy);
	const CSpotLight& operator =(const CSpotLight& _krInstanceToCopy);

private:
	//TVector3 m_vecPosition;
	TVector3 m_vecAttenuation;
	float m_fCutOff;
};
#endif //__SPOTLIGHT_H__