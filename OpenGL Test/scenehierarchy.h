//  File Name   :   CSceneHierarchy.h
//  Description :   Class of CSceneHierarchy
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#ifndef __EDITORINTERPRETER_H__
#define __EDITORINTERPRETER_H__


// Library Includes
#include <rapidxml_utils.hpp>
#include <map>

// Local Includes
#include "editordefines.h"

// Types
enum EResourceType
{
	RESOURCE_INVALID = -1,
	RESOURCE_MODEL,
	RESOURCE_TEXTURE,
	RESOURCE_ANIMATION,
	RESOURCE_MAX
};
struct TPrefabDefinition
{
	TPrefabDefinition(){}
	TPrefabDefinition(	std::string& _rName, 
						std::string& _rModel, 
						std::string& _rTexture, 
						float _pVecScale[3], 
						std::string& _rAIType, 
						bool _bIsAnimated, 
						bool _bIsStatic)
	{
		sName = _rName;
		sModel = _rModel;
		sTexture = _rTexture;
		for (int iScale = 0; iScale < 3; ++iScale)
		{
			vecScale[iScale] = _pVecScale[iScale];
		}

		sAIType = _rAIType;
		bIsAnimated = _bIsAnimated;
		bIsStatic = _bIsStatic;
	}
	std::string sName;
	std::string sModel;
	std::string sTexture;
	float vecScale[3];

	std::string sAIType;
	bool bIsAnimated;
	bool bIsStatic;
};
struct TPrefabInstance
{
	std::string sPrefabName;
	float vecPosition[3];
	float vecScale[3];
	float vecRotation[3];
	float colour[4];
};
struct TLightInformation
{
	TLightInformation(){}
	TLightInformation(std::string& _sLightType, float _pVecPosition[3])
	{
		sLightType = _sLightType;
		for (int iPos = 0; iPos < 3; ++iPos)
		{
			vecPosition[iPos] = _pVecPosition[iPos];
		}
	}
	std::string sLightType;
	float vecPosition[3];
};
struct TEntityNode
{
	TEntityNode()
	: pParent(0)
	{
	}
	void Clear()
	{
		pParent = 0;
		for (unsigned int iChild = 0; iChild < vecChildren.size(); ++iChild)
		{
			vecChildren[iChild]->Clear();
			delete vecChildren[iChild];
			vecChildren[iChild] = 0;
		}
		vecChildren.clear();
		for (unsigned int iLight = 0; iLight < vecLights.size(); ++iLight)
		{
			delete vecLights[iLight];
			vecLights[iLight] = 0;
		}
		vecLights.clear();
	}
	TPrefabInstance tEntity;
	TEntityNode* pParent;
	std::vector<TEntityNode*> vecChildren;
	std::vector<TLightInformation*> vecLights;
};

// Constants

// Prototypes

class CSceneHierarchy
{
	//Member functions
public:
	CSceneHierarchy();
	virtual ~CSceneHierarchy();

	bool Initialise(char* _pcResourceFilename, char* _pcPrefabDefFilename);
	//Load from file
	void LoadResources(char* _pcXMLFilename);
	void LoadPrefabDefinitions(char* _pcXMLFilename);
	void LoadSceneFromXML(char* _pcXMLFilename);
	//Add to hierarchy
	void AddObject(rapidxml::xml_node<>* _pNode, TEntityNode* _pParentNode);
	//Retrieve from hierarchy
	TEntityNode* GetRootNode() const;
	std::string& GetResourceFilename(EResourceType _eResourceType, int _iIndex);
	std::string& GetResourceFilename(EResourceType _eResourceType, std::string& _sResourceName);

	const std::string& GetResourceName(EResourceType _eResourceType, int _iIndex);

	unsigned int GetResourceCount(EResourceType _eResourceType);

//Member variables
private:
	TEntityNode* m_pRootNode;
	std::vector<std::string>* m_pResourceFilenames;
	std::map<std::string, int>* m_pResourceIndexMap;

	std::map<std::string, TPrefabDefinition*> m_mapPrefabs;

};

#endif //__EDITORINTERPRETER_H__