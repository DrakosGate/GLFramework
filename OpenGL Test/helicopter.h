//  File Name   :   helicopter.h
//  Description :   Declaration of helicopter class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __HELICOPTER_H__
#define __HELICOPTER_H__

// Library Includes

// Local Includes
#include "gameobject.h"
#include "chrismaths.h"

// Types

// Constants

// Prototypes
class COpenGLRenderer;
class CModel;
//class CShader;

class CHelicopter : public CGameObject
{
// Member functions
public:
	CHelicopter();
	virtual ~CHelicopter();
	
	virtual bool Initialise(CModel* _pChassisModel, CModel* _pRotorModel, TInputStruct* _pInput);
	virtual void Process(float _fDeltaTick);
	virtual void Draw(COpenGLRenderer* _pRenderer, CCamera* _pCamera, CShader* _pShader);
	void ProcessInput(float _fDeltaTick);
	
	virtual void SetPosition(TVector3& _rVecPosition);

protected:
	CHelicopter(const CHelicopter& _kr);
	CHelicopter& operator= (const CHelicopter& _rhs);

// Member variables
protected:
	CModel* m_pChassis;
	CModel* m_pRotor;
	TInputStruct* m_pInput;
	float m_fRotorRotation;
	float m_fMovementSpeed;
	float m_fRotationSpeed;
	float m_fSpeedBoost;

	float m_fLift;
	float m_fMass;
	float m_fGravity;

	TVector3 m_vecRotorOffset;
	TVector3 m_vecVelocity;	
};

#endif //__HELICOPTER_H__
