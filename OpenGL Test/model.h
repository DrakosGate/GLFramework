//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   model.h
//  Description :   Class of CModel
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __MODEL_H__
#define __MODEL_H__

// Library Includes

// Local Includes
#include "gameobject.h"
#include "defines.h"

// Types

// Constants

// Prototypes
class COpenGLRenderer;
class CTexture;
class CShader;

class CModel : public CGameObject
{
public:
	CModel();
	~CModel();

	virtual bool Initialise(COpenGLRenderer* _pRenderer, TVector3& _rPosition, float _fScale, ETextureType _eTextureType, CShader* _pShader);
	virtual void LoadSquare();
	virtual void LoadFromOBJ(char* _pcFilename);
	virtual void CreateModelData(TVertexType* _pVertices, unsigned int* _pIndices);
	virtual void RecalculateNormals(TVertexType* _pVertices);
	virtual void CleanupBuffers();

	virtual void Draw(COpenGLRenderer* _pRenderer, CCamera* _pCamera);
	virtual void Process(float _fDeltaTime);

	virtual ETextureType GetTextureType() const;
	virtual void SetTextureType(ETextureType _eTextureType);
	virtual TMatrix* GetWorldMatrix();

	ETextureType GetNormalMap() const;
	void SetNormalMap(ETextureType _eNormalMapTexture);

private:
	CModel(const CModel& _krInstanceToCopy);
	const CModel& operator =(const CModel& _krInstanceToCopy);
	
protected:
	COpenGLRenderer* m_pRenderer;
	ETextureType m_eTextureType;
	ETextureType m_eNormalMapTexture;
	CShader* m_pShader;
	int m_iVertexCount;
	int m_iIndexCount;
	unsigned int m_uiVertexArrayID;
	unsigned int m_uiVertexBufferID;
	unsigned int m_uiIndexBufferID;
	float m_fScale;
	float m_fTime;
	
};

#endif //__MODEL_H__