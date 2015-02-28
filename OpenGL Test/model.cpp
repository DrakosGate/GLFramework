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
Model::Model()
:	m_uiVertexCount(0)
,	m_uiIndexCount(0)
,	m_uiVertexArrayID(0)
,	m_uiVertexBufferID(0)
,	m_uiIndexBufferID(0)
,	m_fScale(0.0f)
{
	
}
/**
*
* CModel Destructor
*
* @author Christopher Howlett
*
*/
Model::~Model()
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
Model::Initialise( float _fScale )
{
	m_fScale = _fScale;
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
Model::LoadSquare()
{
	float fHalfScale = m_fScale * 0.5f;
	
	TVertexType* pVertices;
	unsigned int* pIndices;
	m_uiVertexCount = 4;
	m_uiIndexCount = 6;

	pVertices = new TVertexType[m_uiVertexCount];
	pIndices = new unsigned int[m_uiIndexCount];

	//Initialise vertices
	//Bottom left
	pVertices[0].vecPos = glm::vec3(-fHalfScale, fHalfScale, 0.0f);
	pVertices[0].texCoord = glm::vec2(0.0f, 1.0f);
	pVertices[0].vecNormal = glm::vec3(0.0f, 0.0f, 1.0f);
	pVertices[0].vecTangent = glm::vec3(1.0f, 0.0f, 0.0f);
	//Top left
	pVertices[1].vecPos = glm::vec3(-fHalfScale, -fHalfScale, 0.0f);
	pVertices[1].texCoord = glm::vec2(0.0f, 0.0f);
	pVertices[1].vecNormal = glm::vec3(0.0f, 0.0f, 1.0f);
	pVertices[1].vecTangent = glm::vec3(1.0f, 0.0f, 0.0f);
	//Top right
	pVertices[2].vecPos = glm::vec3(fHalfScale, -fHalfScale, 0.0f);
	pVertices[2].texCoord = glm::vec2(1.0f, 0.0f);
	pVertices[2].vecNormal = glm::vec3(0.0f, 0.0f, 1.0f);
	pVertices[2].vecTangent = glm::vec3(1.0f, 0.0f, 0.0f);
	//Bottom right
	pVertices[3].vecPos = glm::vec3(fHalfScale, fHalfScale, 0.0f);
	pVertices[3].texCoord = glm::vec2(1.0f, 1.0f);
	pVertices[3].vecNormal = glm::vec3(0.0f, 0.0f, 1.0f);
	pVertices[3].vecTangent = glm::vec3(1.0f, 0.0f, 0.0f);
	
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
Model::LoadFromOBJ(const char* _pcFilename)
{
	float fHalfScale = m_fScale * 0.5f;
	printf("Loading model %s\n", _pcFilename);
	
	TVertexType* pVertices;
	unsigned int* pIndices;
	ifstream modelFile;
	char cTextBuffer[256];
	
	//Temporary data
	glm::vec3* pPositions;
	glm::vec2* pTexCoords;
	glm::vec3* pNormals;
	glm::vec3* pFaces;
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
		m_uiVertexCount = iNumFaces;
		m_uiIndexCount = iNumFaces;
		pVertices = new TVertexType[m_uiVertexCount];
		pIndices = new unsigned int[m_uiIndexCount];
	
		//Create temporary containers
		pPositions = new glm::vec3[iNumPositions];
		pTexCoords = new glm::vec2[iNumTexCoords];
		pNormals = new glm::vec3[iNumNormals];
		pFaces = new glm::vec3[iNumFaces];
	
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
					modelFile >> pPositions[iCurrentPosition].x >> pPositions[iCurrentPosition].y >> pPositions[iCurrentPosition].z;
					++iCurrentPosition;
				}
				//Texture coordinate data
				if(cStartLetter == 't')
				{
					modelFile >> pTexCoords[iCurrentTex].x >> pTexCoords[iCurrentTex].y;
					++iCurrentTex;
				}
				//Normal data
				if(cStartLetter == 'n')
				{
					modelFile >> pNormals[iCurrentNormal].x >> pNormals[iCurrentNormal].y >> pNormals[iCurrentNormal].z;
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
						modelFile	>> pFaces[iCurrentFace + 2].x >> cInput >> pFaces[iCurrentFace + 2].y >> cInput >> pFaces[iCurrentFace + 2].z
									>> pFaces[iCurrentFace + 1].x >> cInput >> pFaces[iCurrentFace + 1].y >> cInput >> pFaces[iCurrentFace + 1].z
									>> pFaces[iCurrentFace].x >> cInput >> pFaces[iCurrentFace].y >> cInput >> pFaces[iCurrentFace].z;
						iCurrentFace += 3;
					}
				}
			}
		}	
		modelFile.close();
		//Create vertices based on this index order
		for(unsigned int iIndex = 0; iIndex < m_uiIndexCount; ++iIndex)
		{
			pIndices[iIndex] = static_cast<int>(pFaces[iIndex].x) - 1;
			pVertices[iIndex].vecPos	= pPositions[pIndices[iIndex]] * m_fScale;
			pVertices[iIndex].texCoord	= pTexCoords[static_cast<int>(pFaces[iIndex].y) - 1];
			pVertices[iIndex].vecNormal = pNormals[static_cast<int>(pFaces[iIndex].z) - 1];
			normalize( pVertices[iIndex].vecNormal );
			pVertices[iIndex].vecTangent = cross( pVertices[iIndex].vecNormal, glm::vec3(0.0f, 1.0f, 0.0f) );

			pVertices[iIndex].texCoord = glm::vec2( 0.0f, 1.0f ) + ( pVertices[iIndex].texCoord * glm::vec2( 1.0f, -1.0f ) );
		}
		//Draw vertices in ascending order
		for(unsigned int iIndex = 0; iIndex < m_uiIndexCount; ++iIndex)
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
Model::CreateModelData(TVertexType* _pVertices, unsigned int* _pIndices)
{
	//Bind these vertices and indices to index and vertex buffers
	glGenVertexArrays(1, &m_uiVertexArrayID);
	glBindVertexArray(m_uiVertexArrayID);
	glGenBuffers(1, &m_uiVertexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_uiVertexCount * sizeof(TVertexType), _pVertices, GL_STATIC_DRAW);

	//Enable vertex array attributes 
	glEnableVertexAttribArray(0); //POSITION
	glEnableVertexAttribArray(1); //TEXTURE
	glEnableVertexAttribArray(2); //NORMAL
	glEnableVertexAttribArray(3); //TANGENT

	//POSITION
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(TVertexType), 0);

	//TEXTURE
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(TVertexType),  (unsigned char*)(NULL + (3 * sizeof(float))));

	//NORMAL
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(TVertexType),  (unsigned char*)(NULL + (5 * sizeof(float))));
	
	//TANGENT
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(TVertexType),  (unsigned char*)(NULL + (8 * sizeof(float))));

	//Index buffer
	glGenBuffers(1, &m_uiIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexCount * sizeof(unsigned int), _pIndices, GL_STATIC_DRAW);
}
/**
*
* CModel Recalculates vertex normal data
*
* @author Christopher Howlett
*
*/
void 
Model::RecalculateNormals(TVertexType* _pVertices)
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
Model::CleanupBuffers()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//Release vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_uiVertexBufferID);

	//Release Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_uiIndexBufferID);

	//Release vertex array
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_uiVertexArrayID);
}