//  File Name   :   ship.h
//  Description :   Declaration of ship class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __SHIP_H__
#define __SHIP_H__

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

class CShip : public CGameObject
{
// Member functions
public:
	CShip();
	virtual ~CShip();
	
	virtual bool Initialise(CModel* _pChassisModel, TInputStruct* _pInput);
	virtual void Process(float _fDeltaTick);
	virtual void Draw(COpenGLRenderer* _pRenderer, CCamera* _pCamera, CShader* _pShader);
	void ProcessInput(float _fDeltaTick);
	
	virtual void SetPosition(TVector3& _rVecPosition);

protected:
	CShip(const CShip& _kr);
	CShip& operator= (const CShip& _rhs);

// Member variables
protected:
	CModel* m_pChassis;
	TInputStruct* m_pInput;
	float m_fRotorRotation;
	float m_fMovementSpeed;
	float m_fRotationSpeed;
	float m_fSpeedBoost;

	float m_fMass;
	float m_fGravity;

	TVector3 m_vecRotorOffset;
	TVector3 m_vecVelocity;	
};

#endif //__HELICOPTER_H__
