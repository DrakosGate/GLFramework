//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   sphere.cpp
//  Description :   Code for Class CSphere
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <math.h>

// Local Includes

// This Include
#include "sphere.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CSphere constructor
*
* @author Christopher Howlett
*
*/
CSphere::CSphere()
:	m_fRadius(0.0f)
,	m_iRingCount(0)
,	m_iSliceCount(0)
,	m_fMovementSpeed(0.0f)
,	m_fRotationSpeed(0.0f)
{
	m_vecForward = TVector3(0.0f, 0.0f, -1.0f);
}
/**
*
* CSphere Destructor
*
* @author Christopher Howlett
*
*/
CSphere::~CSphere()
{
	
}
/**
*
* CSphere Load Sphere model data
*
* @author Christopher Howlett
*
*/
void 
CSphere::ProcessInput(TInputStruct* _rInput, float _fDeltaTime)
{
	m_fMovementSpeed = 5.0f;
	m_fRotationSpeed = 5.0f;
	//Ensure alt is not pressed
	if(_rInput->bUp)
	{
		m_vecPosition += m_vecForward * m_fMovementSpeed * _fDeltaTime;
	}
	if(_rInput->bDown)
	{
		m_vecPosition -= m_vecForward * m_fMovementSpeed * _fDeltaTime;
	}
	if(_rInput->bLeft)
	{
		m_vecPosition -= m_vecRight * m_fMovementSpeed * _fDeltaTime;
	}
	if(_rInput->bRight)
	{
		m_vecPosition += m_vecRight * m_fMovementSpeed * _fDeltaTime;
	}
	
	//Recreate vectors
	//m_vecForward.Normalise();
	//m_vecUp = TVector3(0.0f, 1.0f, 0.0f);
	//m_vecRight = NVector::Cross(m_vecForward, TVector3(0.0f, 1.0f, 0.0f));
}
/**
*
* CSphere Load Sphere model data
*
* @author Christopher Howlett
*
*/
void 
CSphere::LoadSphere(int _iRings, int _iSlices)
{
	m_fRadius = m_fScale;
	m_iRingCount = _iRings;
	m_iSliceCount = _iSlices;
	
	TVertexType* pVertices;
	unsigned int* pIndices;

	//Top and bottom: 2   +   Slices per ring
	//m_iVertexCount = (m_iSliceCount * (m_iRingCount - 2)) + 2;
	//               Mid sections                              Top and bottom slices
	//m_iIndexCount = ((m_iSliceCount - 1) * (m_iRingCount - 2) * 6) + ((m_iSliceCount - 1) * 3 * 2);
	m_iVertexCount = m_iRingCount * m_iSliceCount * 3;
	m_iIndexCount = m_iRingCount * m_iSliceCount * 6;

	//Setup default vertex data
	pVertices = new TVertexType[m_iVertexCount];
	pIndices = new unsigned int[m_iIndexCount];
	for(int iVertex = 0; iVertex < m_iVertexCount; ++iVertex)
	{
		pVertices[iVertex].vecPos = TVector3(0.0f, 0.0f, 0.0f);
		pVertices[iVertex].vecNormal = TVector3(0.0f, 0.0f, 1.0f);
		pVertices[iVertex].vecTangent = TVector3(1.0f, 0.0f, 0.0f);
		pVertices[iVertex].texCoord = TVector2(0.0f, 1.0f);
	}

	//Create sphere vertices
	float fCurrentSliceOffset = 0.0f;
	float fRingPercentage = 1.0f / static_cast<float>(m_iRingCount);
	float fSlicePercentage = 1.0f / static_cast<float>(m_iSliceCount);
	int iCurrentVertex = 0;
	float fRingAngle = (PI * 2.0f) * fRingPercentage;
	float fSliceAngle = (PI * 2.0f) * fSlicePercentage;
	
	
	for(int iRing = 0; iRing < m_iRingCount; ++iRing)
	{
		for(int iSlice = 0; iSlice < m_iSliceCount; ++iSlice)
		{
			TVector3 vecNewPosition = TVector3( cosf( iSlice * fSliceAngle ) * sinf( fRingAngle * 0.5f * iRing),
												cosf( (0.5f * iRing * fRingAngle)),
												sinf( iSlice * fSliceAngle) * sinf( fRingAngle * 0.5f * iRing) );
			vecNewPosition *= m_fRadius;
			pVertices[iCurrentVertex].vecPos = vecNewPosition;
			pVertices[iCurrentVertex].texCoord = TVector2(iSlice * fSlicePercentage, iRing * fRingPercentage);
			++iCurrentVertex;
		}
	}

	//Create sphere indices
	int iCurrentIndex = 0;
	for(int iRing = 0; iRing < m_iRingCount - 1; ++iRing)
	{
		for(int iSlice = 0; iSlice < m_iSliceCount - 1; ++iSlice)
		{
			//Top triangle
			pIndices[iCurrentIndex++] = iRing * m_iSliceCount + iSlice;
			pIndices[iCurrentIndex++] = iRing * m_iSliceCount + (iSlice + 1);
			pIndices[iCurrentIndex++] = (iRing + 1) * m_iSliceCount + iSlice;
			//Lower triangle
			pIndices[iCurrentIndex++] = (iRing + 1) * m_iSliceCount + iSlice;
			pIndices[iCurrentIndex++] = iRing * m_iSliceCount + (iSlice + 1);
			pIndices[iCurrentIndex++] = (iRing + 1) * m_iSliceCount + (iSlice + 1);
		}
	}

	RecalculateNormals(pVertices);
	CreateModelData(pVertices, pIndices);

	//Delete vertex and index data
	delete[] pVertices;
	delete[] pIndices;
	pVertices = 0;
	pIndices = 0;
}
/**
*
* CSphere GetRadius
*
* @author Christopher Howlett
* @return Returns sphere radius
*
*/
float 
CSphere::GetRadius() const
{
	return m_fRadius;
}