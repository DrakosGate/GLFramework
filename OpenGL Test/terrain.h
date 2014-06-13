//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   terrain.h
//  Description :   Class of CTerrain
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//
#pragma once

#ifndef __TERRAIN_H__
#define __TERRAIN_H__

// Library Includes

// Local Includes
#include "model.h"

// Types

// Constants

// Prototypes
class CTerrain : public CModel
{
//Member functions
public:
	CTerrain();
	virtual ~CTerrain();

	void LoadTerrain(int _iDimensions, float _fTextureTiles);
	int GetIndex(int _iWidth, int _iHeight);
	virtual void RecalculateNormals(TVertexType* _pVertices);
	
private:
	CTerrain(const CTerrain& _krInstanceToCopy);
	const CTerrain& operator =(const CTerrain& _krInstanceToCopy);

//Member variables
protected:
	int m_iWidth;
	int m_iHeight;
};

#endif //__TERRAIN_H__