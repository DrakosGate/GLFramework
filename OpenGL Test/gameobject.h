//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   gameobject.h
//  Description :   Class of CGameObject
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

// Library Includes

// Local Includes
#include "chrismaths.h"

// Types

// Constants

// Prototypes
class COpenGLRenderer;
class CCamera;

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();
	
	virtual bool Initialise();
	virtual void Draw(COpenGLRenderer* _pRenderer, CCamera* _pCamera);
	virtual void Process(float _fDeltaTime);

	//Parent functions
	virtual void ProcessParent(float _fDeltaTime);
	virtual void SetParent(CGameObject* _pParent, TVector3* _pTargetOffset);	

	//PRS Accessor functions
	virtual void SetPosition(TVector3& _rVecPosition);
	virtual TVector3& GetPosition();
	virtual void SetRotation(TVector3& _rRotation);
	virtual TVector3& GetRotation();
	virtual void SetScale(TVector3& _rScale);
	virtual TVector3& GetScale();

	//Vector Accessor functions
	virtual void SetForward(TVector3& _rVecPosition);
	virtual TVector3& GetForward();
	virtual void SetRight(TVector3& _rVecRight);
	virtual TVector3& GetRight();
	virtual void SetUp(TVector3& _rVecUp);
	virtual TVector3& GetUp();
	
private:
	CGameObject(const CGameObject& _kr);
	CGameObject& operator= (const CGameObject& _rhs);
	
protected:
	TMatrix m_matWorld;

	TVector3 m_vecPosition;
	TVector3 m_vecScale;
	TVector3 m_vecRotation;

	TVector3 m_vecForward;
	TVector3 m_vecRight;
	TVector3 m_vecUp;

	bool m_bHasParent;
	CGameObject* m_pParent;
	TVector3 m_vecParentOffset;
};

#endif //__GAMEOBJECT_H__