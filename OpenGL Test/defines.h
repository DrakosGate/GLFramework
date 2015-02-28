//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   defines.h
//  Description :   Definition of general structures and defines
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//
#pragma once

#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <glm/glm.hpp>
#include <cassert>
#include <string>
#include <sstream>

//Program definitions
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

//Helpful defines / functions
#define SAFEDELETE(_object) { if(_object){ delete _object;_object = 0; } }
#define SAFEDELETEARRAY(_array){ if(_array){ delete[] _array; _array = 0; } }
#define ReleaseCOM(_COM) { if(_COM){ _COM->Release();_COM = 0; } }
#define Error(pwcErrorMessage) MessageBox(NULL, pwcErrorMessage, L"ERROR", MB_OK)
#define ErrAssert(expression, pwcErrorMessage) if( !expression ){ MessageBox(NULL, pwcErrorMessage, L"ERROR", MB_OK);  assert( expression ); }

class Model;
class Texture;

template<class T> T ReadFromString( std::string _sInput )
{
	std::stringstream sStream;
	sStream << _sInput;
	T result;
	sStream >> result;
	return result;
}

enum EGameScene
{
	SCENE_INVALID = -1,
	SCENE_PERMANENTSCENE,
	SCENE_3DSCENE,
	SCENE_FINAL,
	SCENE_MAX
};

//Vertex structures
struct TVertexType
{
	glm::vec3 vecPos;
	glm::vec2 texCoord;
	glm::vec3 vecNormal;
	glm::vec3 vecTangent;
};
//Texture types
enum ETextureType
{
	TEXTURE_INVALID = -1,
	TEXTURE_STONE,
	TEXTURE_STONENORMAL,
	TEXTURE_HUMAN,
	TEXTURE_TREE,
	TEXTURE_GRASS, 
	TEXTURE_SQUARES,
	TEXTURE_FLOORTILE,
	TEXTURE_FLOORNORMAL,
	TEXTURE_SHIP,
	TEXTURE_SHIPNORMAL,
	TEXTURE_MAX
};
enum EModelType
{
	MODEL_INVALID = -1,
	MODEL_HELICHASSIS,
	MODEL_HELIROTOR,
	MODEL_SHIP,
	MODEL_LOOP,
	MODEL_CUBE,
	MODEL_MAX
};

//Data Structures
struct TRay
{
	TRay()
	{
		vecPosition *= 0.0f;
		vecDirection *= 0.0f;
	}
	glm::vec3 vecPosition;
	glm::vec3 vecDirection;
};
struct TInputStruct
{
	TInputStruct()
	{
		bUp = false;
		bDown = false;
		bLeft = false;
		bRight = false;
		bW = false;
		bA = false;
		bS = false;
		bD = false;
		bReset = false;
		bMoveRods = false;
		bLeftMouseDown = false;
		bRightMouseDown = false;
		bShift = false;
		bSpace = false;
		bCtrl = false;
		bAlt = false;
		fMouseX = 0.0f;
		fMouseY = 0.0f;
		vecMouseDir *= 0.0f;
		b1 = false;
		b2 = false;
		b3 = false;
		bTilde = false;
		bPlus = false;
		bMinus = false;
	}
	bool bUp;
	bool bDown;
	bool bLeft;
	bool bRight;
	bool bW;
	bool bA;
	bool bS;
	bool bD;
	bool bReset;
	bool bMoveRods;
	bool bLeftMouseDown;
	bool bRightMouseDown;
	bool bShift;
	bool bSpace;
	bool bCtrl;
	bool bAlt;
	float fMouseX;
	float fMouseY;
	bool b1;
	bool b2;
	bool b3;
	bool bTilde;
	bool bPlus;
	bool bMinus;
	glm::vec3 vecMouseDir;
	TRay t3DMouseRay;
};
struct TClothParticle
{
	void Initialise(glm::vec3& _rPosition)
	{
		vecPosition = _rPosition;
		vecPreviousPosition = _rPosition;
		vecStartPosition = _rPosition;
		bIsFixed = false;
	}
	void ResetParticle()
	{
		vecPosition = vecStartPosition;
		vecPreviousPosition = vecStartPosition;
	}
	void SetFixed(bool _bIsFixed)
	{
		bIsFixed = _bIsFixed;
	}
	glm::vec3 vecPosition;
	glm::vec3 vecPreviousPosition;
	glm::vec3 vecStartPosition;
	bool bIsFixed;
};

#endif //DEFINES_H__