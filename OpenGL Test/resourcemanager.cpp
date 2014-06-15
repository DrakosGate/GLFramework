//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   resourcemanager.cpp
//  Description :   Code for Class CResourceManager
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "defines.h"
#include "model.h"
#include "texture.h"
#include "shader.h"
#include "scenehierarchy.h"

// This Include
#include "resourcemanager.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CResourceManager class Constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CResourceManager::CResourceManager()
{
	
}
/**
*
* CResourceManager class Destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CResourceManager::~CResourceManager()
{
	for (unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture)
	{
		delete m_TexturePool[iTexture]->pTexture;
		delete m_TexturePool[iTexture];
		m_TexturePool[iTexture] = 0;
	}
	m_TexturePool.clear();
	
	std::map<std::string, CModel*>::iterator modelIter;
	for (modelIter = m_mapModels.begin(); modelIter != m_mapModels.end(); ++modelIter)
	{
		delete modelIter->second;
		modelIter->second = 0;
	}
}
/**
*
* CResourceManager class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcResourceFilename Filename containing level resources
*
*/
void 
CResourceManager::Initialise(COpenGLRenderer* _pRenderer, CSceneHierarchy* _pSceneHierarchy, CShader* _pObjectShader)
{
	//Loop through models
	printf("\n  == LOADING MODELS\n");
	for (unsigned int iModel = 0; iModel < _pSceneHierarchy->GetResourceCount(RESOURCE_MODEL); ++iModel)
	{
		CModel* pNewModel = new CModel();
		std::string sModelName = _pSceneHierarchy->GetResourceName(RESOURCE_MODEL, iModel);
		std::string sFilename = _pSceneHierarchy->GetResourceFilename(RESOURCE_MODEL, sModelName);
		//Copy filename to buffer
		int iStrLength = sFilename.size() + 1;
		char* pcFilename = new char[iStrLength];
		sprintf_s(pcFilename, iStrLength, "%s", sFilename.c_str());

		pNewModel->Initialise(_pRenderer, TVector3(0.0f, 0.0f, 0.0f), 10.0f, TEXTURE_STONE, _pObjectShader);
		pNewModel->LoadFromOBJ(pcFilename);
		pNewModel->SetNormalMap(TEXTURE_STONENORMAL);
		pNewModel->SetScale(TVector3(1.0f, 1.0f, 1.0f));

		m_mapModels[sModelName] = pNewModel;

		delete[] pcFilename;
		pcFilename = 0;
	}
	
	//Loop through textures
	printf("\n  == LOADING TEXTURES\n");
	for (unsigned int iTexture = 0; iTexture < _pSceneHierarchy->GetResourceCount(RESOURCE_TEXTURE); ++iTexture)
	{
		std::string sTextureName = _pSceneHierarchy->GetResourceName(RESOURCE_TEXTURE, iTexture);
		std::string sTextureFilename = _pSceneHierarchy->GetResourceFilename(RESOURCE_TEXTURE, sTextureName);
		//Concatenate model prefix and model filename
		int iStrLength = sTextureFilename.size() + 1;
		char* pcFilename = new char[iStrLength];
		sprintf_s(pcFilename, iStrLength, "%s", sTextureFilename.c_str());

		//Load new textures from file
		CTexture* pNewTexture = new CTexture();
		pNewTexture->Initialise(_pRenderer, pcFilename, TEXTURE_MAX + iTexture);

		TTexturePoolData* pNewPoolEntry = new TTexturePoolData;
		pNewPoolEntry->pTexture = pNewTexture;
		pNewPoolEntry->sName = sTextureName;
		m_TexturePool.push_back(pNewPoolEntry);

		delete[] pcFilename;
		pcFilename = 0;
	}
	
	//Loop through animations
	//printf("\n  == LOADING ANIMATED MODELS\n");
	//sFilePrefix = pAnimations->first_node("fileprefix")->value();
	//for(rapidxml::xml_node<>* pCurrentAnimation = pAnimations->first_node("model"); pCurrentAnimation; pCurrentAnimation = pCurrentAnimation->next_sibling())
	//{
	//	std::string sAnimationName = pCurrentAnimation->first_attribute("id")->value();
	//	std::string sAnimationFilename = pCurrentAnimation->first_node()->value();
	//	//Concatenate model prefix and model filename
	//	sprintf_s(pcBuffer, iMaxMessageSize, "%s%s", sFilePrefix.c_str(), sAnimationFilename.c_str());
	//
	//	CAnimatedModel* pNewAnim = new CAnimatedModel();
	//	pNewAnim->Initialise();
	//	pNewAnim->LoadAIMesh(_pDevice, 1.0f, pcBuffer);
	//
	//	
	//	//Add to model map 
	//	printf("    = Texture successfully loaded from %s\n", pcBuffer);
	//	m_mapAnimations[sAnimationName] = pNewAnim;
	//}

	
}
/**
*
* CResourceManager class GetModel
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcModelName Name of model
* @return Returns Model specified
*
*/
CModel* 
CResourceManager::GetModel(std::string& _pcModelName) 
{
	return (m_mapModels[_pcModelName]);
}
/**
*
* CResourceManager class GetTexture
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcTextureName Name of texture
* @return Returns Texture specified
*
*/
CTexture* 
CResourceManager::GetTexture(std::string& _pcTextureName) const
{
	//Loop through texture vector and return texture matching this name
	CTexture* pTexture = 0;
	for (unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture)
	{
		if (strcmp(m_TexturePool[iTexture]->sName.c_str(), _pcTextureName.c_str()) == 0)
		{
			pTexture = m_TexturePool[iTexture]->pTexture;
			break;
		}
	}
	if (pTexture == 0)
	{
		//Error(L"Could not find texture in pool!!");
	}
	return (pTexture);
}
/**
*
* CResourceManager class GetTexture
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcTextureName Name of texture
* @return Returns Index of the texture specified
*
*/
int
CResourceManager::GetTextureID(std::string& _pcTextureName) const
{
	//Loop through texture vector and return texture matching this name
	int iTextureID = -1;
	for (unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture)
	{
		if (strcmp(m_TexturePool[iTexture]->sName.c_str(), _pcTextureName.c_str()) == 0)
		{
			iTextureID = iTexture;
			break;
		}
	}
	if (iTextureID == -1)
	{
	//	Error(L"Could not find texture in pool!!");
	}
	return (iTextureID);
}