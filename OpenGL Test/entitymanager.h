//
//  File Name   :   CRenderEntity.h
//  Description :   Class of CRenderEntity
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

// Library Includes
#include <vector>
#include <map>

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes
struct ID3D11Device;
struct ID3D11DeviceContext;
struct TEntityNode;

class Camera;
class Shader;
class BoundingBox;
class AIHiveMind;
class RenderEntity;
class LightManager;

//Structure defined here due to forward declaration
struct TTemporaryEntity
{
	GameObject* pEntity;
	EGameScene eScene;
};

class EntityManager
{
//Member functions
public:
	EntityManager();
	virtual ~EntityManager();

	virtual bool Initialise();

	virtual void Process( float _fDeltaTime, EGameScene _eGameScene);
	virtual void Draw( Camera* _pCurrentCamera, EGameScene _eScene);
	
	void AddEntity( GameObject* _pNewEntity, EGameScene _eScene );
	bool IsEntityInFrustum( Camera* _pCamera, BoundingBox* _pBoundingBox);

	void ClearScene(EGameScene _eScene);

private:
	EntityManager(const EntityManager& _krInstanceToCopy);
	const EntityManager& operator =(const EntityManager& _krInstanceToCopy);

//Member variables
protected:
	std::vector<GameObject*>* m_pRenderEntities;
	std::vector<GameObject*>* m_pTransparentEntities;
	std::vector<TPrefabOptions*> m_vecPrefabTypes;

	std::map<std::string, unsigned int> m_mapPrefabIndex;

	float* m_pCameraDepths;
	int m_iTotalTransparentCount;
	
};


#endif //ENTITY_MANAGER_H__