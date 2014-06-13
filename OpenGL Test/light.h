//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   light.h
//  Description :   Class of CLight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __LIGHT_H__
#define __LIGHT_H__

// Library Includes

// Local Includes
#include "gameobject.h"
#include "chrismaths.h"

// Types

// Constants

// Prototypes

class CLight : public CGameObject
{
public:
	CLight();
	virtual ~CLight();

	virtual void Initialise(TVector3& _rVecDir, TVector4& _rVecColour, float _fSpecularPower);
	virtual void SetColour(TVector4& _rVecColour);
	virtual void SetDirection(TVector3& _rVecDir);
	virtual void ProcessParent(float _fDeltaTime);
	
	virtual TVector4& GetColour();
	virtual TVector3& GetDirection();
	virtual float GetSpecularPower() const;

	virtual bool IsOn() const;
	virtual void ToggleIsOn(bool _bIsOn);

private:
	CLight(const CLight& _kr);
	CLight& operator= (const CLight& _rhs);

protected:
	TVector3 m_vecDirection;
	TVector4 m_vecColour;
	float m_fSpecularPower;
	bool m_bIsOn;
};

#endif //__LIGHT_H__