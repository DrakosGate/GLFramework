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
#include <glm/glm.hpp>

// Local Includes
#include "defines.h"
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
	void SetView3D( glm::vec3& _rVecRight, glm::vec3& _rVecUp, glm::vec3& _rVecDir, glm::vec3& _rVecPos);
	void SetViewLookAt(glm::vec3& _rVecPos, glm::vec3& _rVecLook, glm::vec3& _rVecUp);
	void SetViewMatrix(glm::mat4x4& _rMatView);
	void SetProjMatrix(glm::mat4x4& _rMatProj);
	
	glm::mat4x4& GetViewMatrix();
	glm::mat4x4& GetProjectionMatrix();
	glm::mat4x4& GetWorldMatrix();

	void Calculate3DMouseCoordinates(glm::vec2& _rVecMouse, TRay& _rRay);

protected:
	CCamera(const CCamera& _kr);
	CCamera& operator= (const CCamera& _rhs);
// Member variables
protected:
	ECameraType m_eCameraType;
	TInputStruct* m_pInput;

	glm::vec3 m_vecVelocity;
	float m_fMovementSpeed;
	float m_fRotationSpeed;
	float m_fSpeedBoost;
	
	glm::mat4x4 m_matView;
	glm::mat4x4 m_matProj;
	glm::mat4x4 m_matWorld;

	float m_fFOV;
	float m_fNearClip;
	float m_fFarClip;
	float m_fAspectRatio;

	int m_iScreenWidth;
	int m_iScreenHeight;
};

#endif //CAMERA_H__
