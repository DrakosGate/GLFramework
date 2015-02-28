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
#include "resource.h"

// Types

// Constants

// Prototypes
class OpenGLRenderer;
class Texture;
class Shader;

class Model : public Resource
{
public:
	Model();
	virtual ~Model();

	virtual bool Initialise( float _fScale );
	virtual void LoadSquare();
	virtual void LoadFromOBJ(const char* _pcFilename);

	virtual void CreateModelData(TVertexType* _pVertices, unsigned int* _pIndices);
	virtual void RecalculateNormals(TVertexType* _pVertices);
	virtual void CleanupBuffers();

	const unsigned int GetVertexBuffer( ) const { return m_uiVertexArrayID; } 
	const unsigned int GetIndexBuffer( ) const { return m_uiIndexBufferID; }
	const unsigned int GetVertexCount( ) const { return m_uiVertexCount; }
	const unsigned int GetIndexCount( ) const { return m_uiIndexCount; }

private:
	Model(const Model& _krInstanceToCopy);
	const Model& operator =(const Model& _krInstanceToCopy);
	
protected:
	unsigned int m_uiVertexCount;
	unsigned int m_uiIndexCount;
	unsigned int m_uiVertexArrayID;
	unsigned int m_uiVertexBufferID;
	unsigned int m_uiIndexBufferID;
	float m_fScale;
	
};

#endif //__MODEL_H__