//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   springlink.cpp
//  Description :   Code for Class CSpringLink
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes

// This Include
#include "springlink.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CSpringLink class Constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CSpringLink::CSpringLink()
:	m_fRestDistance(0.0f)
,	m_pStartPoint(0)
,	m_pEndPoint(0)
,	m_bIsConnected(false)
,	m_pPartnerLink(0)
{
	m_vecLink *= 0.0f;
}
/**
*
* CSpringLink class Destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CSpringLink::~CSpringLink()
{
	
}
/**
*
* CSpringLink class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns success of initialisation
*
*/
bool 
CSpringLink::Initialise(TClothParticle* _pStartPoint, TClothParticle* _pEndPoint, float _fLinkScale)
{
	m_bIsConnected = true;

	bool bResult = true;
	m_pStartPoint = _pStartPoint;
	m_pEndPoint = _pEndPoint;
	
	m_fStiffness = 0.6f * _fLinkScale;

	m_vecLink = _pEndPoint->vecPosition - _pStartPoint->vecPosition;
	m_fRestDistance = NVector::SqMagnitude(m_vecLink) * 1.0f;
	m_fBreakingPoint = m_fRestDistance * 100.0f;

	return bResult;
}
/**
*
* CSpringLink class Process
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void 
CSpringLink::Process(float _fDeltaTime)
{
	if(m_bIsConnected)
	{
		if(m_pStartPoint->bIsFixed && m_pEndPoint->bIsFixed)
		{
			//This point is fixed - do not process this link
		}
		else
		{
			m_vecLink = m_pEndPoint->vecPosition - m_pStartPoint->vecPosition;
			float fLinkDistance = NVector::SqMagnitude(m_vecLink);
			if(fLinkDistance > m_fBreakingPoint)
			{
				m_bIsConnected = false;
				//Check if this link has a partner link, if so break that link
				if(m_pPartnerLink)
				{
					m_pPartnerLink->ToggleIsConnected(false);
				}
			}
			else
			{
				float fDifference = (fLinkDistance - m_fRestDistance) / fLinkDistance;
				//Process constraint behaviour
				if(m_pStartPoint->bIsFixed == false)
				{
					m_pStartPoint->vecPosition += m_vecLink * m_fStiffness * fDifference * _fDeltaTime;
				}
				if(m_pEndPoint->bIsFixed == false)
				{
					m_pEndPoint->vecPosition -= m_vecLink * m_fStiffness * fDifference * _fDeltaTime;
				}
			}
		}
	}
}
/**
*
* CSpringLink class IsConnected
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns whether or not link is still connected
*
*/
bool 
CSpringLink::IsConnected() const
{
	return m_bIsConnected;
}
/**
*
* CSpringLink class Toggle Connected state
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _bIsConnected Connected boolean state
*
*/
void 
CSpringLink::ToggleIsConnected(bool _bIsConnected)
{
	m_bIsConnected = _bIsConnected;
}

/**
*
* CSpringLink class Has Fixed Point
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns whether or not link has a fixed point
*
*/
bool 
CSpringLink::HasFixedPoint() const
{
	return (m_pStartPoint->bIsFixed || m_pEndPoint->bIsFixed);
}
/**
*
* CSpringLink class GetPosition
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns midway point between start and end points
*
*/
TVector3 
CSpringLink::GetPosition()
{
	return (m_pStartPoint->vecPosition + (m_vecLink * 0.5f));
} 
/**
*
* CSpringLink class Gets Start Point
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns Start point
*
*/
TClothParticle*
CSpringLink::GetStart()
{
	return m_pStartPoint;
}
/**
*
* CSpringLink class Gets End Point
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns End point
*
*/
TClothParticle* 
CSpringLink::GetEnd()
{
	return m_pEndPoint;
}
/**
*
* CSpringLink class Sets the Partner link of this link - is broken at the same time as this link
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pPartner Partner link of this link
*
*/
void
CSpringLink::SetPartner(CSpringLink* _pPartner)
{
	m_pPartnerLink = _pPartner;
}