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

#include "chrismaths.h"

//Program definitions
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

//Helpful defines / functions
#define Error(pwcErrorMessage) MessageBox(NULL, pwcErrorMessage, L"ERROR", MB_OK)

//Vertex structures
struct TVertexType
{
	TVector3 vecPos;
	TVector2 texCoord;
	TVector3 vecNormal;
	TVector3 vecTangent;
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
	TVector3 vecPosition;
	TVector3 vecDirection;
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
	TVector3 vecMouseDir;
	TRay t3DMouseRay;
};
struct TClothParticle
{
	void Initialise(TVector3& _rPosition)
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
	TVector3 vecPosition;
	TVector3 vecPreviousPosition;
	TVector3 vecStartPosition;
	bool bIsFixed;
};

#endif //DEFINES_H__