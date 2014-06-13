//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   springlink.h
//  Description :   Class of CParticleSystem
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __SPRINGLINK_H__
#define __SPRINGLINK_H__

// Library Includes

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes

class CSpringLink
{
//Member functions
public:
	CSpringLink();
	virtual ~CSpringLink();

	virtual bool Initialise(TClothParticle* _pStartPoint, TClothParticle* _pEndPoint, float _fLinkScale);
	virtual void Process(float _fDeltaTime);
	bool IsConnected() const;
	void ToggleIsConnected(bool _bIsConnected);
	bool HasFixedPoint() const;
	TVector3 GetPosition();
	TClothParticle* GetStart();
	TClothParticle* GetEnd();
	void SetPartner(CSpringLink* _pPartner);
	
private:
	CSpringLink(const CSpringLink& _krInstanceToCopy);
	const CSpringLink& operator =(const CSpringLink& _krInstanceToCopy);

//Member variables
protected:
	TClothParticle* m_pStartPoint;
	TClothParticle* m_pEndPoint;
	CSpringLink* m_pPartnerLink;
	TVector3 m_vecLink;
	float m_fRestDistance;
	float m_fStiffness;
	bool m_bIsConnected;
	float m_fBreakingPoint;

	
};


#endif //__CLOTH_H__