//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   texture.h
//  Description :   Declaration of Texture class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

// Library Includes

// Local Includes

// Types

// Constants

// Prototypes
class COpenGLRenderer;

class CTexture
{
//Member functions
public:
	CTexture();
	~CTexture();

	bool Initialise(COpenGLRenderer* _pRenderer, char* _pcFilename, unsigned int _uiTextureUnit);
	bool LoadFromTarga(COpenGLRenderer* _pRenderer, char* _pcFilename, unsigned int _uiTextureUnit);
	
private:
	CTexture(const CTexture& _krInstanceToCopy);
	const CTexture& operator =(const CTexture& _krInstanceToCopy);

//Member variables
private:
	COpenGLRenderer* m_pRenderer;
	bool m_bIsLoaded;
	unsigned int m_uiTextureID;
	int m_iTextureDimensions;

};

#endif //__TEXTURE_H__
