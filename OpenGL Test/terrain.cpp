//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   terrain.cpp
//  Description :   Code for Class CTerrain
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <math.h>

// Local Includes

// This Include
#include "terrain.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CTerrain class Constructor
*
* @author Christopher Howlett
*
*/
CTerrain::CTerrain()
:	m_iWidth(0)
{

}
/**
*
* CTerrain class destructor
*
* @author Christopher Howlett
*
*/
CTerrain::~CTerrain()
{

}
/**
*
* CTerrain class LoadTerrain
*
* @author Christopher Howlett
* @param _iDimensions Terrain Dimensions
* @param _fTextureTiles TextureTiling amount
*
*/
void 
CTerrain::LoadTerrain(int _iDimensions, float _fTextureTiles)
{
	TVertexType* pVertices;
	unsigned int* pIndices;
	int iNumFaces = 0;
	m_iWidth = _iDimensions;
	m_iHeight = _iDimensions;
	float fScalePercent = m_fScale / static_cast<float>(m_iWidth);
	float fSizePercent = 1.0f / static_cast<float>(m_iWidth);
	float fUVPercentage = fSizePercent * _fTextureTiles;

	m_iVertexCount = m_iWidth * m_iWidth;
	iNumFaces = ((m_iHeight - 1) * (m_iWidth - 1)) * 2;
	//Initialise vertices
	pVertices = new TVertexType[m_iVertexCount];
	for(int iHeight = 0; iHeight < m_iHeight; ++iHeight)
	{
		for(int iWidth = 0; iWidth < m_iWidth; ++iWidth)
		{
			pVertices[GetIndex(iWidth, iHeight)].vecPos = TVector3(fScalePercent * iWidth, 0.0f, fScalePercent * iHeight) - TVector3(m_fScale * 0.5f, 0.0f, m_fScale * 0.5f);
			pVertices[GetIndex(iWidth, iHeight)].texCoord = TVector2(fUVPercentage * iWidth, fUVPercentage * iHeight);
			pVertices[GetIndex(iWidth, iHeight)].vecNormal = TVector3(0.0f, 1.0f, 0.0f);
			pVertices[GetIndex(iWidth, iHeight)].vecNormal.Normalise();
			pVertices[GetIndex(iWidth, iHeight)].vecTangent = NVector::Cross(pVertices[GetIndex(iWidth, iHeight)].vecNormal, TVector3(0.0f, 1.0f, 0.0f));
		}
	}

	//Recalculate normals after height offsets
	RecalculateNormals(pVertices);

	//Initialise indices
	int iCurrentIndex = 0;
	m_iIndexCount = iNumFaces * 3;
	pIndices = new unsigned int[m_iIndexCount];
	for(int iHeight = 0; iHeight < m_iHeight - 1; ++iHeight)
	{
		for(int iWidth = 0; iWidth < m_iWidth - 1; ++iWidth)
		{
			//Top face
			pIndices[iCurrentIndex++] = GetIndex(iWidth, iHeight);
			pIndices[iCurrentIndex++] = GetIndex(iWidth + 1, iHeight);
			pIndices[iCurrentIndex++] = GetIndex(iWidth, iHeight + 1);

			//Bottom face
			pIndices[iCurrentIndex++] = GetIndex(iWidth, iHeight + 1);
			pIndices[iCurrentIndex++] = GetIndex(iWidth + 1, iHeight);
			pIndices[iCurrentIndex++] = GetIndex(iWidth + 1, iHeight + 1);
		}
	}

	CreateModelData(pVertices, pIndices);

	delete[] pVertices;
	delete[] pIndices;
	pVertices = 0;
	pIndices = 0;
}
/**
*
* CTerrain class GetIndex
*
* @author Christopher Howlett
* @param _iWidth Width index
* @param _iHeight Height index
* @return Returns this index
*
*/
int 
CTerrain::GetIndex(int _iWidth, int _iHeight)
{
	return ((m_iWidth * _iHeight) + _iWidth);
}
/**
*
* CTerrain class RecalculateNormals
*
* @author Christopher Howlett
* @param _pVertices Terrain mesh Vertices 
*
*/
void 
CTerrain::RecalculateNormals(TVertexType* _pVertices)
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
			iThisIndex = GetIndex(iWidth, iHeight);
			iVerticesFound = 0;
			//TOP LEFT
			if(iHeight - 1 >= 0 && iWidth - 1 >= 0)
			{
				vecTopLeftVertex	= NVector::Cross(	_pVertices[iThisIndex - m_iHeight].vecPos - _pVertices[iThisIndex].vecPos,
														_pVertices[iThisIndex - 1].vecPos - _pVertices[iThisIndex].vecPos);
				vecVertexNormal += vecTopLeftVertex;
				++iVerticesFound;
			}
			//TOP RIGHT
			if(iHeight - 1 >= 0 && iWidth + 1 < m_iWidth)
			{
				vecTopRightVertex	= NVector::Cross(	_pVertices[iThisIndex + 1].vecPos - _pVertices[iThisIndex].vecPos,
														_pVertices[iThisIndex - m_iHeight].vecPos - _pVertices[iThisIndex].vecPos);
				vecVertexNormal += vecTopRightVertex;
				++iVerticesFound;
			}
			//BOTTOM LEFT
			if(iWidth - 1 >= 0 && iHeight + 1 < m_iHeight)
			{
				vecBottomLeftVertex	= NVector::Cross(	_pVertices[iThisIndex - 1].vecPos - _pVertices[iThisIndex].vecPos,
														_pVertices[iThisIndex + m_iHeight].vecPos - _pVertices[iThisIndex].vecPos);
				vecVertexNormal += vecBottomLeftVertex;
				++iVerticesFound;
			}
			//BOTTOM RIGHT
			if(iWidth + 1 < m_iWidth && iHeight + 1 < m_iHeight)
			{
				vecBottomRightVertex	= NVector::Cross(	_pVertices[iThisIndex + m_iHeight].vecPos - _pVertices[iThisIndex].vecPos,
															_pVertices[iThisIndex + 1].vecPos - _pVertices[iThisIndex].vecPos);
				vecVertexNormal += vecBottomRightVertex;
				++iVerticesFound;
			}
			_pVertices[iThisIndex].vecNormal = vecVertexNormal; 
			_pVertices[iThisIndex].vecNormal /= static_cast<float>(iVerticesFound);
			_pVertices[iThisIndex].vecNormal.Normalise();
			_pVertices[iThisIndex].vecTangent = NVector::Cross(_pVertices[iThisIndex].vecNormal, TVector3(0.0f, 0.0f, 1.0f));;
		}
	}
}