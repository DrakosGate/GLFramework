//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   CWireCloth.cpp
//  Description :   Code for Class CWireCloth
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "springlink.h"
#include "openglrenderer.h"
#include "sphere.h"

// This Include
#include "wirecloth.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CWireCloth class Constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CWireCloth::CWireCloth()
:	m_pClothParticles(0)
,	m_pLinks(0)
,	m_fWidthPercentage(0.0f)
,	m_iNumParticles(0)
,	m_iWidth(0)
,	m_iHeight(0)
,	m_iLinkCount(0)
,	m_bHasGrabbedParticle(false)
,	m_iSelectedParticle(0)
,	m_fClothDrag(0.0f)
,	m_pIndices(0)
,	m_pVertices(0)
,	m_fGroundHeight(0.0f)
,	m_fClothThickness(0.0f)
,	m_fHitDistance(0.0f)
,	m_fMaxParticleDistance(0.0f)
,	m_fRodContraction(0.0f)
,	m_pRodArray(0)
,	m_iNumRods(0)
,	m_pRodStartDistance(0)
,	m_fGroundFriction(0.0f)
,	m_fMinimumPickingDistance(0.0f)
,	m_eInputType(INPUT_INVALID)
,	m_bIsIndividualRod(false)
{
	m_vecTargetPosition *= 0.0f;
	m_vecTotalForce *= 0.0f;
	m_vecScale = TVector2(1.0f, 1.0f);
}
/**
*
* CWireCloth class Destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CWireCloth::~CWireCloth()
{
	if(m_pRodArray)
	{
		delete[] m_pRodArray;
		m_pRodArray = 0;
		delete[] m_pRodStartDistance;
		m_pRodStartDistance = 0;
	}
	if(m_pVertices)
	{
		delete[] m_pVertices;
		m_pVertices = 0;		
	}
	if(m_pIndices)
	{
		delete[] m_pIndices;
		m_pIndices = 0;
	}
	if(m_pClothParticles)
	{
		delete[] m_pClothParticles;
		m_pClothParticles = 0;
	}
	if(m_pLinks)
	{
		delete[] m_pLinks;
		m_pLinks = 0;
	}
}
/**
*
* CWireCloth class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns success of initialisation
*
*/
bool 
CWireCloth::Initialise(COpenGLRenderer* _pRenderer, int _iWidth, int _iHeight, TVector3& _rPosition, TVector2& _rScale, TVector2& _rVecTiling, float _fGroundHeight)
{
	m_pRenderer = _pRenderer;
	m_eInputType = INPUT_CLOTHDRAG;

	bool bResult = true;
	m_iNumParticles = _iWidth * _iHeight;
	m_iVertexCount = _iWidth * _iHeight;
	m_iWidth = _iWidth;
	m_iHeight = _iHeight;
	m_iIndexCount = ((_iWidth - 1) * (_iHeight - 1) * 5); 
	m_vecScale = _rScale;
	m_fClothDrag = 1.0f;
	m_fGroundFriction = 0.99f;
	m_fMouseDragSpeed = 50.0f;
	m_fClothThickness = 1.1f;
	m_fMaxParticleDistance = 0.5f * m_vecScale.fX / static_cast<float>(m_iWidth);
	m_fGroundHeight = _fGroundHeight + m_fClothThickness;
	m_fMinimumPickingDistance = 50.0f;
	
	m_vecPosition = _rPosition;

	// Create cube vertices
	TVector3 defaultNormal = TVector3(0.0f, 0.0f, -1.0f);
	TVector3 binormal = TVector3(0.0f, 0.0f, 1.0f);
	TVector3 tangent = TVector3(0.0f, 0.0f, 1.0f);

	m_fWidthPercentage = 1.0f / static_cast<float>(m_iWidth);
	m_pClothParticles = new TClothParticle[m_iNumParticles];
	m_pVertices = new TVertexType[m_iVertexCount];
	//Create particle positions
	int iParticle = 0;
	for(int iHeight = 0; iHeight < m_iHeight; ++iHeight)
	{
		for(int iWidth = 0; iWidth < m_iWidth; ++iWidth)
		{
			TVector3 vecNewPosition = TVector3(	(iWidth * m_fWidthPercentage * m_vecScale.fX) - m_vecScale.fX * 0.5f + _rPosition.fX, 
												(-iHeight * m_fWidthPercentage * m_vecScale.fY) - m_vecScale.fY * 0.5f + _rPosition.fY, 
												sinf(static_cast<float>(-iHeight * 5.0f) * 0.01f));
			m_pClothParticles[iParticle].Initialise(vecNewPosition);
			m_pVertices[iParticle].vecPos = m_pClothParticles[iParticle].vecPosition; 
			m_pVertices[iParticle].vecNormal = defaultNormal; 
			m_pVertices[iParticle].vecTangent =	tangent; 
			m_pVertices[iParticle].texCoord = TVector2(static_cast<float>(iWidth) * m_fWidthPercentage * _rVecTiling.fX, static_cast<float>(iHeight) * m_fWidthPercentage * _rVecTiling.fY);
			++iParticle;
		}
	}
	RecalculateNormals(m_pVertices);

	//Create fixed points
	m_fRodContraction = 1.0f;
	m_iNumRods = 7;
	m_pRodArray = new TClothParticle*[m_iNumRods];
	m_pRodStartDistance = new TVector3[m_iNumRods];
	m_pRodArray[0] = &m_pClothParticles[0];
	m_pRodArray[1] = &m_pClothParticles[m_iWidth - 1];
	m_pRodArray[2] = &m_pClothParticles[static_cast<int>(m_iWidth * 0.3f)];
	m_pRodArray[3] = &m_pClothParticles[static_cast<int>(m_iWidth * 0.6f)];
	m_pRodArray[4] = &m_pClothParticles[static_cast<int>(m_iWidth * 0.15f)];
	m_pRodArray[5] = &m_pClothParticles[static_cast<int>(m_iWidth * 0.45f)];
	m_pRodArray[6] = &m_pClothParticles[static_cast<int>(m_iWidth * 0.8f)];
	for(int iRod = 0; iRod < m_iNumRods; ++iRod)
	{
		m_pRodArray[iRod]->bIsFixed = true;
		m_pRodStartDistance[iRod] = m_pRodArray[iRod]->vecPosition - m_pRodArray[0]->vecPosition;
	}

	//Initialise indices
	int iCurrentIndex = 0;
	m_pIndices = new unsigned int[m_iIndexCount];
	
	//Create particle links based on index order
	m_iLinkCount =	((m_iWidth - 1) * m_iHeight) + //Horizontal links
					((m_iHeight - 1) * m_iWidth);
	m_pLinks = new CSpringLink[m_iLinkCount];
	int iLink = 0;
	for(int iHeight = 0; iHeight < m_iHeight; ++iHeight)
	{
		for(int iWidth = 0; iWidth < m_iWidth; ++iWidth)
		{
			//Right face
			if(iWidth < m_iWidth - 1)
			{
				m_pLinks[iLink++].Initialise(&m_pClothParticles[Get2DIndex(iWidth, iHeight)], &m_pClothParticles[Get2DIndex(iWidth + 1, iHeight)], m_vecScale.fX);
				m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth, iHeight);
				m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth + 1, iHeight);
			}
			
			//Bottom face
			if(iHeight < m_iHeight - 1)
			{
				m_pLinks[iLink++].Initialise(&m_pClothParticles[Get2DIndex(iWidth, iHeight)], &m_pClothParticles[Get2DIndex(iWidth, iHeight + 1)], m_vecScale.fX);
				m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth, iHeight);
				m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth, iHeight + 1);		
			}
		}
	}

	CreateModelData(m_pVertices, m_pIndices);
	
	return bResult;
}
/**
*
* CWireCloth class Process
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void 
CWireCloth::Process(float _fDeltaTime)
{
	//Process particle behaviour on all non-fixed particles
	TVector3 vecGravity = TVector3(0.0f, -9.8f, 0.0f);
	TVector3 vecForceDirection = m_vecTotalForce;
	vecForceDirection.Normalise();
	for(int iParticle = 0; iParticle < m_iNumParticles; ++iParticle)
	{
		//Check of particle is a fixed particle
		if(m_pClothParticles[iParticle].bIsFixed)
		{
			//Do nothing - this is fixed
		}
		else
		{
			//Check dot product with total applied force
			float fForceDirection = NVector::Dot(vecForceDirection, m_pVertices[iParticle].vecNormal);
			//Maintain minimum influence of force due to thickness of cloth
			if(fForceDirection < m_fClothThickness)
			{
				fForceDirection = m_fClothThickness;
			}
			//Process Verlet Integration with total applied force + gravity
			ProcessVerletIntegration(m_pClothParticles[iParticle], (m_vecTotalForce * fForceDirection) + vecGravity, _fDeltaTime);
		}
	}
	//Check if this particle is being dragged
	if(m_bHasGrabbedParticle)
	{
		TVector3 vecToTarget = m_vecTargetPosition - m_pClothParticles[m_iSelectedParticle].vecPosition;
		vecToTarget.Normalise();
		TClothParticle* pSelectedParticle = m_pLinks[m_iSelectedParticle].GetStart();
		pSelectedParticle->vecPosition += vecToTarget * m_fMouseDragSpeed *_fDeltaTime;
	}

	//Process cloth links
	ProcessLinks(_fDeltaTime);

	//Process cloth collisions
	ProcessCollisions(_fDeltaTime);

	//Calculate world matrix
	NMatrix::Transformation(m_matWorld, m_vecPosition, TVector3(1.0f, 1.0f, 1.0f), m_vecRotation);
	
	m_vecTotalForce *= 0.0f;
}
/**
*
* CWireCloth class ProcessInput
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pKeys User Input structure
* @param _fDT Game time elapsed
*
*/
void 
CWireCloth::ProcessInput(TInputStruct& _pKeys, float _fDT)
{
	if(_pKeys.bLeftMouseDown)
	{
		//Check which input type is currently active
		switch(m_eInputType)
		{
		case INPUT_CLOTHDRAG:
			{
				//Drag only if first time clicking - not holding down
				if(m_bPreviousMouseClick == false)
				{
					ProcessClothDrag(_pKeys, _fDT);
				}
			}
			break;
		case INPUT_CLOTHTEAR:
			{
				ProcessClothTear(_pKeys, _fDT);
			}
			break;
		case INPUT_MOUSEFORCE:
			{
				ProcessClothDrag(_pKeys, _fDT);
			}
			break;
		}
		//Set target position for moving input types
		m_vecTargetPosition = _pKeys.t3DMouseRay.vecPosition + (_pKeys.t3DMouseRay.vecDirection * m_fHitDistance);
	}
	else if(_pKeys.bLeftMouseDown == false)
	{
		m_bHasGrabbedParticle = false;
	}
	if(_pKeys.bReset)
	{
		ResetCloth();
	}
	if(_pKeys.b1)
	{
		m_eInputType = INPUT_CLOTHDRAG;
	}
	if(_pKeys.b2)
	{
		m_eInputType = INPUT_CLOTHTEAR;
	}
	if(_pKeys.b3)
	{
		m_eInputType = INPUT_MOUSEFORCE;
	}
	if(_pKeys.bMoveRods)
	{
		//Move rods to the right
		if(_pKeys.vecMouseDir.fX > 0.0f)
		{
			m_fRodContraction += _fDT;
		}
		//Move rods to the left
		else
		{
			m_fRodContraction -= _fDT;
		}
		//Move all rods to their target positions
		for(int iRod = 1; iRod < m_iNumRods; ++iRod)
		{
			m_pRodArray[iRod]->vecPosition += ((m_pRodArray[0]->vecPosition + m_pRodStartDistance[iRod] * m_fRodContraction) - m_pRodArray[iRod]->vecPosition) * _fDT;
		}
	}
	m_bPreviousMouseClick = _pKeys.bLeftMouseDown;
}
/**
*
* CWireCloth Draw
*
* @author Christopher Howlett
* @param _pCamera Camera used to draw this model
*
*/
void 
CWireCloth::Draw(COpenGLRenderer* _pRenderer, CCamera* _pCamera)
{
	_pRenderer->glBindVertexArray(m_uiVertexArrayID);

	glDrawElements(GL_LINES, m_iIndexCount, GL_UNSIGNED_INT, 0);
}
/**
*
* CWireCloth ProcessVerletIntegration
*
* @author Christopher Howlett
* @param _rParticle Particle processed in function
* @param _rVecTotalForce Total force applied on this particle
* @param _fDeltaTime Game Time Elapsed
*
*/
void 
CWireCloth::ProcessVerletIntegration(TClothParticle& _rParticle, TVector3& _rVecTotalForce, float _fDeltaTime)
{
	//Ensure particle does not leave world
	float fWorldSize = 50.0f;
	if(fabs(_rParticle.vecPosition.fX) < fWorldSize && fabs(_rParticle.vecPosition.fY) < fWorldSize && fabs(_rParticle.vecPosition.fZ) < fWorldSize)
	{
		float fDrag = m_fClothDrag;
		//Add ground friction to cloth drag if cloth is close to the ground
		if(fabs(_rParticle.vecPosition.fY - m_fGroundHeight) < m_fClothThickness)
		{
			fDrag = m_fClothDrag * m_fGroundFriction;
		}
		//Process verlet integration using cloth drag
		TVector3 vecNewPosition =	((_rParticle.vecPosition * (1.0f + fDrag)) - // (1 + drag)POScurrent
									(_rParticle.vecPreviousPosition * fDrag)) +  // (drag)POSprevious + 
									(_rVecTotalForce * (_fDeltaTime * _fDeltaTime));	// total force * Time
		_rParticle.vecPreviousPosition = _rParticle.vecPosition;
		_rParticle.vecPosition = vecNewPosition;
	}
	else
	{
		//Particle has left world - do not process
	}
}
/**
*
* CWireCloth Process Collisions on all cloth particles
*
* @author Christopher Howlett
* @param _fDeltaTime Game Time Elapsed
*
*/
void 
CWireCloth::ProcessCollisions(float _fDeltaTime)
{
	for(int iParticle = 0; iParticle < m_iNumParticles; ++iParticle)
	{
		//Check collisions with other particles
		for(int iOtherPart = iParticle; iOtherPart < m_iNumParticles; ++iOtherPart)
		{
			if(iOtherPart != iParticle)
			{
				TVector3 vecToParticle = m_pClothParticles[iOtherPart].vecPosition - m_pClothParticles[iParticle].vecPosition;
				float fDistanceToParticle = NVector::SqMagnitude(vecToParticle);
				if(fDistanceToParticle < m_fMaxParticleDistance)
				{
					m_pClothParticles[iOtherPart].vecPosition += vecToParticle * _fDeltaTime;
					m_pClothParticles[iParticle].vecPosition -= vecToParticle * _fDeltaTime;
				}
			}
		}
		//Check collisions with cloth
		if(m_pClothParticles[iParticle].vecPosition.fY < m_fGroundHeight)
		{
			m_pClothParticles[iParticle].vecPosition.fY = m_fGroundHeight;
		}
	}
}
/**
*
* CWireCloth class Processes Collisions with a sphere
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pSphere Sphere collider
* @param _fDeltaTime Game Time Elapsed
*
*/
void 
CWireCloth::ProcessSphereCollisions(CSphere* _pSphere, float _fDeltaTime)
{
	float fSphereRadiusSq = _pSphere->GetRadius();
	fSphereRadiusSq *= fSphereRadiusSq;
	TVector3 vecRelativeSpherePosition = _pSphere->GetPosition() - m_vecPosition;
	for(int iParticle = 0; iParticle < m_iNumParticles; ++iParticle)
	{
		TVector3 vecToSphere = vecRelativeSpherePosition - (m_pClothParticles[iParticle].vecPosition);
		float fDistanceToSphere = NVector::SqMagnitude(vecToSphere);
		if(fDistanceToSphere < (m_fMaxParticleDistance + fSphereRadiusSq))
		{
			m_pClothParticles[iParticle].vecPosition -= vecToSphere * _fDeltaTime;
		}
	}
}
/**
*
* CWireCloth class ProcessLinks - Process cloth links
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void 
CWireCloth::ProcessLinks(float _fDeltaTime)
{
	//Cycle through all links
	for(int iLink = 0; iLink < m_iLinkCount; ++iLink)
	{
		//Check if link is still connected
		if(m_pLinks[iLink].IsConnected())
		{
			m_pLinks[iLink].Process(_fDeltaTime);
		}
		else
		{
			//This link is broken!
		}
	}
}
/**
*
* CWireCloth class Processes Cloth Dragging with mouse
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pKeys User Input structure
* @param _fDeltaTime Game time elapsed
*
*/
void 
CWireCloth::ProcessClothDrag(TInputStruct& _pKeys, float _fDeltaTime)
{
	TVector3 vecMousePos = _pKeys.t3DMouseRay.vecPosition - m_vecPosition;
	TLine mouseRay = TLine(vecMousePos, vecMousePos + (_pKeys.t3DMouseRay.vecDirection * 100.0f));
	float fClosestDistance = m_fMinimumPickingDistance;
	for(int iLink = 0; iLink < m_iLinkCount; ++iLink)
	{
		//Exclude broken links from check
		if(m_pLinks[iLink].IsConnected())
		{
			//Check closest distance from this point to the line
			TVector3 vecLinkPos = m_pLinks[iLink].GetPosition() - (m_vecPosition * 0.5f);
			float fPercentAlongLine = NCollision::PointToLine(	vecLinkPos, 
																mouseRay);
			TVector3 vecPointOnLine = mouseRay.vecPointA + ((mouseRay.vecPointB - mouseRay.vecPointA) * fPercentAlongLine);
			//Compare distance to closest distance
			float fDistanceToParticle = NVector::SqMagnitude(vecPointOnLine - vecLinkPos);
			if(fDistanceToParticle < fClosestDistance)
			{
				m_bHasGrabbedParticle = true;
				m_iSelectedParticle = iLink;
				fClosestDistance = fDistanceToParticle;
				m_fHitDistance = fClosestDistance;
			}
		}
	}
}
/**
*
* CWireCloth class Processes Cloth Tearing with mouse
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pKeys User Input structure
* @param _fDeltaTime Game time elapsed
*
*/
void 
CWireCloth::ProcessClothTear(TInputStruct& _pKeys, float _fDeltaTime)
{
	TVector3 vecMousePos = _pKeys.t3DMouseRay.vecPosition - m_vecPosition;
	TLine mouseRay = TLine(vecMousePos, vecMousePos + (_pKeys.t3DMouseRay.vecDirection * 100.0f));
	float fClosestDistance = m_fMinimumPickingDistance;
	for(int iLink = 0; iLink < m_iLinkCount; ++iLink)
	{
		//Exclude broken links from check
		if(m_pLinks[iLink].IsConnected())
		{
			//Check closest distance from this point to the line
			TVector3 vecLinkPos = m_pLinks[iLink].GetPosition() - (m_vecPosition * 0.5f);
			float fPercentAlongLine = NCollision::PointToLine(	vecLinkPos, 
																mouseRay);
			TVector3 vecPointOnLine = mouseRay.vecPointA + ((mouseRay.vecPointB - mouseRay.vecPointA) * fPercentAlongLine);
			//Compare distance to closest distance
			float fDistanceToParticle = NVector::SqMagnitude(vecPointOnLine - vecLinkPos);
			if(fDistanceToParticle < fClosestDistance)
			{
				m_iSelectedParticle = iLink;
				fClosestDistance = fDistanceToParticle;
				m_fHitDistance = fClosestDistance;
			}
		}
	}
	//Break this link
	m_pLinks[m_iSelectedParticle].ToggleIsConnected(false);
}
/**
*
* CWireCloth class Returns input type currently active
*
* @author Christopher Howlett
* @return Returns input type currently active
*
*/
EInputType 
CWireCloth::GetInputType() const
{
	return m_eInputType;
}
/**
*
* CWireCloth class Get2DIndex Returns 2D index
*
* @author Christopher Howlett
* @param _iX X Coordinate
* @param _iY Y Coordinate
* @return Returns 2D index of coordinates
*
*/
int 
CWireCloth::Get2DIndex(int _iX, int _iY)
{
	return ((_iY * m_iWidth) + _iX);
}
/**
*
* CWireCloth class Resets Cloth particles
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pVertices Cloth Vertex data
*
*/
void 
CWireCloth::ResetCloth()
{
	for(int iParticle = 0; iParticle < m_iNumParticles; ++iParticle)
	{
		m_pClothParticles[iParticle].ResetParticle();
	}
	for(int iLink = 0; iLink < m_iLinkCount; ++iLink)
	{
		m_pLinks[iLink].ToggleIsConnected(true);
	}
}
/**
*
* CWireCloth class Recalculates Cloth Normals
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pVertices Cloth Vertex data
*
*/
void 
CWireCloth::RecalculateNormals(TVertexType* _pVertices)
{
	TVector3 vecTopLeftVertex;
	TVector3 vecTopRightVertex;
	TVector3 vecBottomLeftVertex;
	TVector3 vecBottomRightVertex;

	TVector3 vecVertexNormal;
	int iVerticesFound = 0;
	int iThisIndex = 0;
	for(int iHeight = 0; iHeight < m_iHeight; ++iHeight)
	{
		for(int iWidth = 0; iWidth < m_iWidth; ++iWidth)
		{
			iThisIndex = Get2DIndex(iWidth, iHeight);
			iVerticesFound = 0;
			//TOP LEFT
			if(iHeight - 1 >= 0 && iWidth - 1 >= 0)
			{
				vecTopLeftVertex	= NVector::Cross(	_pVertices[iThisIndex - m_iWidth].vecPos - _pVertices[iThisIndex].vecPos,
														_pVertices[iThisIndex - 1].vecPos - _pVertices[iThisIndex].vecPos);
				vecVertexNormal += vecTopLeftVertex;
				++iVerticesFound;
			}
			//TOP RIGHT
			if(iHeight - 1 >= 0 && iWidth + 1 < m_iWidth)
			{
				vecTopRightVertex	= NVector::Cross(	_pVertices[iThisIndex + 1].vecPos - _pVertices[iThisIndex].vecPos,
														_pVertices[iThisIndex - m_iWidth].vecPos - _pVertices[iThisIndex].vecPos);
				vecVertexNormal += vecTopRightVertex;
				++iVerticesFound;
			}
			//BOTTOM LEFT
			if(iWidth - 1 >= 0 && iHeight + 1 < m_iHeight)
			{
				vecBottomLeftVertex	= NVector::Cross(	_pVertices[iThisIndex - 1].vecPos - _pVertices[iThisIndex].vecPos,
														_pVertices[iThisIndex + m_iWidth].vecPos - _pVertices[iThisIndex].vecPos);
				vecVertexNormal += vecBottomLeftVertex;
				++iVerticesFound;
			}
			//BOTTOM RIGHT
			if(iWidth + 1 < m_iWidth && iHeight + 1 < m_iHeight)
			{
				vecBottomRightVertex	= NVector::Cross(	_pVertices[iThisIndex + m_iWidth].vecPos - _pVertices[iThisIndex].vecPos,
															_pVertices[iThisIndex + 1].vecPos - _pVertices[iThisIndex].vecPos);
				vecVertexNormal += vecBottomRightVertex;
				++iVerticesFound;
			}
			if(iVerticesFound > 0)
			{
				_pVertices[iThisIndex].vecNormal = vecVertexNormal; 
				_pVertices[iThisIndex].vecNormal /= static_cast<float>(iVerticesFound);
				_pVertices[iThisIndex].vecNormal.Normalise();
				_pVertices[iThisIndex].vecTangent = NVector::Cross(_pVertices[iThisIndex].vecNormal, TVector3(0.0f, 1.0f, 0.0f));
			}
		}
	}
}
/**
*
* CWireCloth class Recreates Cloth Dynamic Buffer
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CWireCloth::RecreateDynamicBuffer()
{
	//Recreate vertex and index buffers
	CleanupBuffers();
	for(int iParticle = 0; iParticle < m_iNumParticles; ++iParticle)
	{
		m_pVertices[iParticle].vecPos = m_pClothParticles[iParticle].vecPosition;
	}
	//Initialise indices
	int iCurrentIndex = 0;
	int iLink = 0;
	for(int iHeight = 0; iHeight < m_iHeight; ++iHeight)
	{
		for(int iWidth = 0; iWidth < m_iWidth; ++iWidth)
		{
			//Right face
			if(iWidth < m_iWidth - 1)
			{
				if(m_pLinks[iLink].IsConnected())
				{
					m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth, iHeight);
					m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth + 1, iHeight);
				}
				else
				{
					m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth, iHeight);
					m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth, iHeight);
				}
				++iLink;
			}
			
			//Bottom face
			if(iHeight < m_iHeight - 1)
			{
				if(m_pLinks[iLink].IsConnected())
				{
					m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth, iHeight);
					m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth, iHeight + 1);		
				}
				else
				{
					m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth, iHeight);
					m_pIndices[iCurrentIndex++] = Get2DIndex(iWidth, iHeight);
				}
				++iLink;
			}
		}
	}	
	RecalculateNormals(m_pVertices);
	
	//m_pRenderer->glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	//void* pVertexData = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	//if(pVertexData)
	//{
	//	glUnmapBuffer(GL_ARRAY_BUFFER);
	//}
	//glUnmapBuffer(GL_ARRAY_BUFFER);
	//m_pRenderer->glBindBuffer(GL_ARRAY_BUFFER, 0);
	//void* pVertexData = reinterpret_cast<void*>(&m_pVertices);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TVertexType), pVertexData);
	//glBindBuffer(GL_ARRAY_BUFFER, NULL);
	
	//void* pIndexData = reinterpret_cast<void*>(&m_pIndices);
	//glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(unsigned int), pIndexData);
	//glBindBuffer(GL_ARRAY_BUFFER, NULL);
	
	CreateModelData(m_pVertices, m_pIndices);
}
/**
*
* CWireCloth Sets cloth target position
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rVecPosition New position set
*
*/
void 
CWireCloth::SetPosition(TVector3& _rVecPosition)
{
	m_vecTargetPosition = _rVecPosition;
}
/**
*
* CWireCloth Applies force on cloth
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rForce Add this force to total
*
*/
void 
CWireCloth::AddForce(TVector3& _rForce)
{
	m_vecTotalForce += _rForce;
}