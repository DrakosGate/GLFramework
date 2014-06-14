//
//  File Name   :   resourcemanager.h
//  Description :   Class of CResourceManager
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

// Library Includes
#include <vector>
#include <map>

// Local Includes
#include "defines.h"

// Types
class CTexture;
struct TTexturePoolData
{
	CTexture* pTexture;
	std::string sName;
};

// Constants
#define MAX_TEXTURE_POOL_SIZE 20

// Prototypes
struct ID3D11Device;

class COpenGLRenderer;
class CSceneHierarchy;
class CTexture;
class CModel;
class CShader;

class CResourceManager
{
//Member functions
public:
	CResourceManager();
	virtual ~CResourceManager();

	virtual void Initialise(COpenGLRenderer* _pRenderer, CSceneHierarchy* _pSceneHierarchy, CShader* _pObjectShader);
	
	CModel*  GetModel(std::string& _pcModelName) ;
	CTexture* GetTexture(std::string& _pcTextureName) const;
	int GetTextureID(std::string& _pcTextureName) const;
	
private:
	CResourceManager(const CResourceManager& _krInstanceToCopy);
	const CResourceManager& operator =(const CResourceManager& _krInstanceToCopy);

//Member variables
protected:
	std::vector<TTexturePoolData*> m_TexturePool;
	std::map<std::string, CModel*> m_mapModels;
	
	CTexture** m_pTextureArray;

};


#endif //__RESOURCE_MANAGER_H__