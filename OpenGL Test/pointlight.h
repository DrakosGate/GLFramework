//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   CPointLight.h
//  Description :   Class of CPointLight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __POINTLIGHT_H__
#define __POINTLIGHT_H__

// Library Includes

// Local Includes
#include "light.h"

// Types

// Constants

// Prototypes

class CPointLight : public CLight
{
public:
	CPointLight();
	virtual ~CPointLight();

	virtual bool Initialise(TVector3& _rVecPosition, TVector4& _rVecColour, TVector3& _rAttenuation, float _fSpecularPower);
	
	TVector3& GetAttenuation();
	void SetAttenuation(TVector3& _rAttenuation);

private:
	CPointLight(const CPointLight& _krInstanceToCopy);
	const CPointLight& operator =(const CPointLight& _krInstanceToCopy);


private:
	TVector3 m_vecAttenuation;
};

#endif //__POINTLIGHT_H__