//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   sphere.h
//  Description :   Class of CSphere
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __SPHERE_H__
#define __SPHERE_H__

// Library Includes

// Local Includes
#include "model.h"

// Types

// Constants

// Prototypes
class COpenGLRenderer;
class CTexture;
class CShader;

class CSphere : public CModel
{
public:
	CSphere();
	virtual ~CSphere();

	virtual void ProcessInput(TInputStruct* _rInput, float _fDeltaTime);
	virtual void LoadSphere(int _iRings, int _iSlices);
	virtual float GetRadius() const;
	
private:
	CSphere(const CSphere& _krInstanceToCopy);
	const CSphere& operator =(const CSphere& _krInstanceToCopy);
	
protected:
	float m_fMovementSpeed;
	float m_fRotationSpeed;

	float m_fRadius;	
	int m_iRingCount;
	int m_iSliceCount;

};

#endif //__SPHERE_H__