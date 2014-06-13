//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   camera.h
//  Description :   Declaration of camera class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

// Library Includes

// Local Includes
#include "defines.h"
#include "chrismaths.h"
#include "gameobject.h"

// Types
enum ECameraType
{
	CAMERA_INVALID = -1,
	CAMERA_ORTHOGONAL,
	CAMERA_PERSPECTIVE,
	CAMERA_MAX
};

// Constants

// Prototypes

class CCamera : public CGameObject
{
// Member functions
public:
	CCamera();
	virtual ~CCamera();
	
	virtual bool Initialise(float _fFOV,
							float _fAspectRatio,
							float _fNearClip,
							float _fFarClip,
							float _fMovementSpeed,
							float _fRotationSpeed,
							int _iScreenWidth, 
							int _iScreenHeight,
							ECameraType _eCameraType);
	virtual void Process(float _fDeltaTick);
	void ProcessInput(float _fDeltaTick);
	virtual void ProcessParent(float _fDeltaTime);
	
	void SetInput(TInputStruct* _pInput);
	void SetClipPlanes(float _fNear, float _fFar);
	float GetNearPlane() const;
	float GetFarPlane() const;

	void SetFOV(float _fFOV);
	void SetView3D( TVector3& _rVecRight, TVector3& _rVecUp, TVector3& _rVecDir, TVector3& _rVecPos);
	void SetViewLookAt(TVector3& _rVecPos, TVector3& _rVecLook, TVector3& _rVecUp);
	void SetViewMatrix(TMatrix& _rMatView);
	void SetProjMatrix(TMatrix& _rMatProj);
	
	TMatrix& GetViewMatrix();
	TMatrix& GetProjectionMatrix();
	TMatrix& GetWorldMatrix();

	void Calculate3DMouseCoordinates(TVector2& _rVecMouse, TRay& _rRay);

protected:
	CCamera(const CCamera& _kr);
	CCamera& operator= (const CCamera& _rhs);
// Member variables
protected:
	ECameraType m_eCameraType;
	TInputStruct* m_pInput;

	TVector3 m_vecVelocity;
	float m_fMovementSpeed;
	float m_fRotationSpeed;
	float m_fSpeedBoost;
	
	TMatrix m_matView;
	TMatrix m_matProj;
	TMatrix m_matWorld;

	float m_fFOV;
	float m_fNearClip;
	float m_fFarClip;
	float m_fAspectRatio;

	int m_iScreenWidth;
	int m_iScreenHeight;
};

#endif //CAMERA_H__
