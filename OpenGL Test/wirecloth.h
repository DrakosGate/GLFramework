//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   CWireCloth.h
//  Description :   Class of CWireCloth
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __WIRECLOTH_H__
#define __WIRECLOTH_H__

// Library Includes

// Local Includes
#include "chrismaths.h"
#include "model.h"
#include "defines.h"

// Types
enum EInputType
{
	INPUT_INVALID = -1,
	INPUT_CLOTHDRAG,
	INPUT_CLOTHTEAR,
	INPUT_MOUSEFORCE,
	INPUT_RODDRAG,
	INPUT_MAX
};

// Constants

// Prototypes
class CCamera;
class CSpringLink;
class COpenGLRenderer;
class CSphere;

class CWireCloth : public CModel
{
//Member functions
public:
	CWireCloth();
	virtual ~CWireCloth();

	virtual bool Initialise(COpenGLRenderer* _pRenderer, int _iWidth, int _iHeight, TVector3& _rPosition, TVector2& _rScale, TVector2& _rVecTiling, float _fGroundHeight);
	virtual void Process(float _fDeltaTime);
	virtual void ProcessInput(TInputStruct& _pKeys, float _fDT);
	virtual void Draw(COpenGLRenderer* _pRenderer, CCamera* _pCamera);
	void ProcessVerletIntegration(TClothParticle& _rParticle, TVector3& _rVecTotalForce, float _fDeltaTime);
	void ProcessCollisions(float _fDeltaTime);
	void ProcessSphereCollisions(CSphere* _pSphere, float _fDeltaTime);
	void ProcessLinks(float _fDeltaTime);
	//Input types
	void ProcessClothDrag(TInputStruct& _pKeys, float _fDeltaTime);
	void ProcessClothTear(TInputStruct& _pKeys, float _fDeltaTime);
	EInputType GetInputType() const;

	int Get2DIndex(int _iX, int _iY);
	void ResetCloth();
	void RecreateDynamicBuffer();
	void RecalculateNormals(TVertexType* _pVertices);

	virtual void SetPosition(TVector3& _rVecPosition);
	void AddForce(TVector3& _rForce);

private:
	CWireCloth(const CWireCloth& _krInstanceToCopy);
	const CWireCloth& operator =(const CWireCloth& _krInstanceToCopy);

//Member variables
protected:
	TVertexType* m_pVertices;
	unsigned int* m_pIndices;

	TClothParticle* m_pClothParticles;
	CSpringLink* m_pLinks;
	TVector3 m_vecTargetPosition;
	TVector3 m_vecTotalForce;
	int m_iLinkCount;
	TVector2 m_vecScale; 
	float m_fWidthPercentage;
	int m_iNumParticles;
	int m_iWidth;
	int m_iHeight;
	float m_fClothDrag;
	float m_fGroundFriction;
	float m_fMouseDragSpeed;
	float m_fMaxParticleDistance;

	//Mouse interaction
	EInputType m_eInputType;
	bool m_bHasGrabbedParticle;
	bool m_bPreviousMouseClick;
	int m_iSelectedParticle;
	float m_fHitDistance;
	float m_fMinimumPickingDistance;

	float m_fGroundHeight;
	float m_fClothThickness;
	float m_fRodContraction;
	TClothParticle** m_pRodArray;
	TVector3* m_pRodStartDistance;
	int m_iNumRods;
	bool m_bIsIndividualRod;
};


#endif //__WIRECLOTH_H__