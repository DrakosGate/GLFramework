//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   CModel.cpp
//  Description :   Code for Class CModel
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <fstream>

// Local Includes
#include "openglrenderer.h"
#include "texture.h"
#include "shader.h"

// This Include
#include "model.h"

// Static Variables

// Static Function Prototypes

// Implementation
using namespace std;
/**
*
* CModel constructor
*
* @author Christopher Howlett
*
*/
CModel::CModel()
:	m_iVertexCount(0)
,	m_iIndexCount(0)
,	m_uiVertexArrayID(0)
,	m_uiVertexBufferID(0)
,	m_uiIndexBufferID(0)
,	m_pRenderer(0)
,	m_eTextureType(TEXTURE_INVALID)
,	m_eNormalMapTexture(TEXTURE_INVALID)
,	m_pShader(0)
,	m_fTime(0.0f)
,	m_fScale(0.0f)
{
	m_vecPosition *= 0.0f;
	m_vecRotation *= 0.0f;

	m_vecScale.fX = 1.0f;
	m_vecScale.fY = 1.0f;
	m_vecScale.fZ = 1.0f;

	NMatrix::Identity(m_matWorld);
}
/**
*
* CModel Destructor
*
* @author Christopher Howlett
*
*/
CModel::~CModel()
{
	//Release vertex and index buffers
	CleanupBuffers();
}
/**
*
* CModel Initialisation
*
* @author Christopher Howlett
*
*/
bool 
CModel::Initialise(COpenGLRenderer* _pRenderer, TVector3& _rPosition, float _fScale, ETextureType _eTextureType, CShader* _pShader)
{
	m_pRenderer = _pRenderer;
	m_vecPosition = _rPosition;
	m_fScale = _fScale;
	m_eTextureType = _eTextureType;
	m_pShader = _pShader;

	return true;
}
/**
*
* CModel Load Square model data
*
* @author Christopher Howlett
*
*/
void 
CModel::LoadSquare()
{
	float fHalfScale = m_fScale * 0.5f;
	
	TVertexType* pVertices;
	unsigned int* pIndices;
	m_iVertexCount = 4;
	m_iIndexCount = 6;

	pVertices = new TVertexType[m_iVertexCount];
	pIndices = new unsigned int[m_iIndexCount];

	//Initialise vertices
	//Bottom left
	pVertices[0].vecPos = TVector3(-fHalfScale, fHalfScale, 0.0f) + m_vecPosition;
	pVertices[0].texCoord = TVector2(0.0f, 1.0f);
	pVertices[0].vecNormal = TVector3(0.0f, 0.0f, 1.0f);
	pVertices[0].vecTangent = TVector3(1.0f, 0.0f, 0.0f);
	//Top left
	pVertices[1].vecPos = TVector3(-fHalfScale, -fHalfScale, 0.0f) + m_vecPosition;
	pVertices[1].texCoord = TVector2(0.0f, 0.0f);
	pVertices[1].vecNormal = TVector3(0.0f, 0.0f, 1.0f);
	pVertices[1].vecTangent = TVector3(1.0f, 0.0f, 0.0f);
	//Top right
	pVertices[2].vecPos = TVector3(fHalfScale, -fHalfScale, 0.0f) + m_vecPosition;
	pVertices[2].texCoord = TVector2(1.0f, 0.0f);
	pVertices[2].vecNormal = TVector3(0.0f, 0.0f, 1.0f);
	pVertices[2].vecTangent = TVector3(1.0f, 0.0f, 0.0f);
	//Bottom right
	pVertices[3].vecPos = TVector3(fHalfScale, fHalfScale, 0.0f) + m_vecPosition;
	pVertices[3].texCoord = TVector2(1.0f, 1.0f);
	pVertices[3].vecNormal = TVector3(0.0f, 0.0f, 1.0f);
	pVertices[3].vecTangent = TVector3(1.0f, 0.0f, 0.0f);
	
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 2;
	pIndices[4] = 3;
	pIndices[5] = 0;

	CreateModelData(pVertices, pIndices);

	//Delete vertex and index data
	delete[] pVertices;
	delete[] pIndices;
	pVertices = 0;
	pIndices = 0;
}
/**
*
* CModel Load model data from an OBJ file
*
* @author Christopher Howlett
*
*/
void 
CModel::LoadFromOBJ(char* _pcFilename)
{
	float fHalfScale = m_fScale * 0.5f;
	printf("Loading model %s\n", _pcFilename);
	
	TVertexType* pVertices;
	unsigned int* pIndices;
	ifstream modelFile;
	char cTextBuffer[256];
	
	//Temporary data
	TVector3* pPositions;
	TVector2* pTexCoords;
	TVector3* pNormals;
	TVector3* pFaces;
	int iNumPositions = 0;
	int iNumFaces = 0;
	int iNumNormals = 0;
	int iNumTexCoords = 0;
	
	//Open model file
	modelFile.open(_pcFilename);
	if(modelFile.is_open())
	{
		//Count vertices
		while(!modelFile.eof())
		{
			modelFile.getline(cTextBuffer, 256);
			if(cTextBuffer[0] == 'v')
			{
				//Found a vertex
				if(cTextBuffer[1] == ' ')
				{
					++iNumPositions;
				}
				//Texture coordinate data
				else if(cTextBuffer[1] == 't')
				{
					++iNumTexCoords;
				}
				//Normal data
				else if(cTextBuffer[1] == 'n')
				{
					++iNumNormals;
				}
			}
			//Face data
			else if(cTextBuffer[0] == 'f')
			{
				iNumFaces += 3;
			}
	
		}
		modelFile.close();
		modelFile.open(_pcFilename);
		//Create vertex and index array
		m_iVertexCount = iNumFaces;
		m_iIndexCount = iNumFaces;
		pVertices = new TVertexType[m_iVertexCount];
		pIndices = new unsigned int[m_iIndexCount];
	
		//Create temporary containers
		pPositions = new TVector3[iNumPositions];
		pTexCoords = new TVector2[iNumTexCoords];
		pNormals = new TVector3[iNumNormals];
		pFaces = new TVector3[iNumFaces];
	
		//Counters
		int iCurrentPosition = 0;
		int iCurrentTex = 0;
		int iCurrentNormal = 0;
		int iCurrentFace = 0;
		char cStartLetter;
		char cInput;
		while(!modelFile.eof())
		{
			modelFile.get(cStartLetter);
			//Starts with 'v'
			if(cStartLetter == 'v')
			{
				modelFile.get(cStartLetter);
				//Position data
				if(cStartLetter == ' ')
				{
					modelFile >> pPositions[iCurrentPosition].fX >> pPositions[iCurrentPosition].fY >> pPositions[iCurrentPosition].fZ;
					++iCurrentPosition;
				}
				//Texture coordinate data
				if(cStartLetter == 't')
				{
					modelFile >> pTexCoords[iCurrentTex].fX >> pTexCoords[iCurrentTex].fY;
					++iCurrentTex;
				}
				//Normal data
				if(cStartLetter == 'n')
				{
					modelFile >> pNormals[iCurrentNormal].fX >> pNormals[iCurrentNormal].fY >> pNormals[iCurrentNormal].fZ;
					++iCurrentNormal;
				}
			}
			//Faces
			else if(cStartLetter == 'f')
			{
				modelFile.get(cStartLetter);
				if(cStartLetter == ' ')
				{
					//Account for backwards winding
					if(iCurrentFace < iNumFaces)
					{
						modelFile	>> pFaces[iCurrentFace + 2].fX >> cInput >> pFaces[iCurrentFace + 2].fY >> cInput >> pFaces[iCurrentFace + 2].fZ
									>> pFaces[iCurrentFace + 1].fX >> cInput >> pFaces[iCurrentFace + 1].fY >> cInput >> pFaces[iCurrentFace + 1].fZ
									>> pFaces[iCurrentFace].fX >> cInput >> pFaces[iCurrentFace].fY >> cInput >> pFaces[iCurrentFace].fZ;
						iCurrentFace += 3;
					}
				}
			}
		}	
		modelFile.close();
		//Create vertices based on this index order
		for(int iIndex = 0; iIndex < m_iIndexCount; ++iIndex)
		{
			pIndices[iIndex] = static_cast<int>(pFaces[iIndex].fX) - 1;
			pVertices[iIndex].vecPos	= pPositions[pIndices[iIndex]] * m_fScale;
			pVertices[iIndex].texCoord	= pTexCoords[static_cast<int>(pFaces[iIndex].fY) - 1];
			pVertices[iIndex].vecNormal = pNormals[static_cast<int>(pFaces[iIndex].fZ) - 1];
			pVertices[iIndex].vecNormal.Normalise();
			pVertices[iIndex].vecTangent = NVector::Cross(pVertices[iIndex].vecNormal, TVector3(0.0f, 1.0f, 0.0f));
		}
		//Draw vertices in ascending order
		for(int iIndex = 0; iIndex < m_iIndexCount; ++iIndex)
		{
			pIndices[iIndex] = iIndex;
		}
	
		//Delete temporary data
		delete[] pTexCoords;
		delete[] pNormals;
		delete[] pPositions;
		delete[] pFaces;
		pFaces = 0;
		pPositions = 0;
		pTexCoords = 0;
		pNormals = 0;
		
		CreateModelData(pVertices, pIndices);
	}
	else
	{
		Error(L"Could not open model file");
	}	
	
	//Delete vertex and index data
	delete[] pVertices;
	delete[] pIndices;
	pVertices = 0;
	pIndices = 0;
}
/**
*
* CModel Load model data into Vertex and Index buffers
*
* @author Christopher Howlett
*
*/
void 
CModel::CreateModelData(TVertexType* _pVertices, unsigned int* _pIndices)
{
	//Bind these vertices and indices to index and vertex buffers
	m_pRenderer->glGenVertexArrays(1, &m_uiVertexArrayID);
	m_pRenderer->glBindVertexArray(m_uiVertexArrayID);
	m_pRenderer->glGenBuffers(1, &m_uiVertexBufferID);

	m_pRenderer->glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	m_pRenderer->glBufferData(GL_ARRAY_BUFFER, m_iVertexCount * sizeof(TVertexType), _pVertices, GL_STATIC_DRAW);

	//Enable vertex array attributes 
	m_pRenderer->glEnableVertexAttribArray(0); //POSITION
	m_pRenderer->glEnableVertexAttribArray(1); //TEXTURE
	m_pRenderer->glEnableVertexAttribArray(2); //NORMAL
	m_pRenderer->glEnableVertexAttribArray(3); //TANGENT

	//POSITION
	m_pRenderer->glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	m_pRenderer->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(TVertexType), 0);

	//TEXTURE
	m_pRenderer->glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	m_pRenderer->glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(TVertexType),  (unsigned char*)(NULL + (3 * sizeof(float))));

	//NORMAL
	m_pRenderer->glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	m_pRenderer->glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(TVertexType),  (unsigned char*)(NULL + (5 * sizeof(float))));
	
	//TANGENT
	m_pRenderer->glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	m_pRenderer->glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(TVertexType),  (unsigned char*)(NULL + (8 * sizeof(float))));

	//Index buffer
	m_pRenderer->glGenBuffers(1, &m_uiIndexBufferID);
	m_pRenderer->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexBufferID);
	m_pRenderer->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iIndexCount * sizeof(unsigned int), _pIndices, GL_STATIC_DRAW);
}
/**
*
* CModel Recalculates vertex normal data
*
* @author Christopher Howlett
*
*/
void 
CModel::RecalculateNormals(TVertexType* _pVertices)
{
	//Do nothing for this model - inherited classes perform different calculations
}
/**
*
* CModel Cleanup vertex and index buffers
*
* @author Christopher Howlett
*
*/
void 
CModel::CleanupBuffers()
{
	m_pRenderer->glDisableVertexAttribArray(0);
	m_pRenderer->glDisableVertexAttribArray(1);

	//Release vertex buffer
	m_pRenderer->glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_pRenderer->glDeleteBuffers(1, &m_uiVertexBufferID);

	//Release Index buffer
	m_pRenderer->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	m_pRenderer->glDeleteBuffers(1, &m_uiIndexBufferID);

	//Release vertex array
	m_pRenderer->glBindVertexArray(0);
	m_pRenderer->glDeleteVertexArrays(1, &m_uiVertexArrayID);
}
/**
*
* CModel Process
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void 
CModel::Process(float _fDeltaTime)
{
	NMatrix::Transformation(m_matWorld, m_vecPosition, m_vecScale, m_vecRotation);
}
/**
*
* CModel Draw
*
* @author Christopher Howlett
* @param _pCamera Camera used to draw this model
*
*/
void 
CModel::Draw(COpenGLRenderer* _pRenderer, CCamera* _pCamera)
{
	_pRenderer->glBindVertexArray(m_uiVertexArrayID);

	glDrawElements(GL_TRIANGLES, m_iIndexCount, GL_UNSIGNED_INT, 0);
}
/**
*
* CModel GetTextureType
*
* @author Christopher Howlett
* @return Returns Model texture type
*
*/
ETextureType 
CModel::GetTextureType() const
{
	return m_eTextureType;
}
/**
*
* CModel SetTextureType
*
* @author Christopher Howlett
* @param _eTextureType Sets Model texture type
*
*/
void  
CModel::SetTextureType(ETextureType _eTextureType)
{
	m_eTextureType = _eTextureType;
}
/**
*
* CModel GetWorldMatrix
*
* @author Christopher Howlett
* @return Returns Model WorldMatrix
*
*/
TMatrix* 
CModel::GetWorldMatrix()
{
	return &m_matWorld;
}
/**
*
* CModel GetNormalMap
*
* @author Christopher Howlett
* @return Returns Model Normal Map Texture
*
*/
ETextureType 
CModel::GetNormalMap() const
{
	return m_eNormalMapTexture;
}
/**
*
* CModel SetNormalMap
*
* @author Christopher Howlett
* @param _eNormalMapTexture Sets Normal Map
*
*/
void 
CModel::SetNormalMap(ETextureType _eNormalMapTexture)
{
	m_eNormalMapTexture = _eNormalMapTexture;
}