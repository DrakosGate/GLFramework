//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   texture.cpp
//  Description :   Code for CTexture class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <iostream>
#include <stdio.h>

// Local Includes
#include "openglrenderer.h"
#include "texturetypes.h"

// This Include
#include "texture.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CTexture class Constructor
*
* @author Christopher Howlett
*
*/
CTexture::CTexture()
:	m_uiTextureID(0)
,	m_bIsLoaded(false)
,	m_pRenderer(0)
,	m_iTextureDimensions(0)
{

}
/**
*
* CTexture class destructor
*
* @author Christopher Howlett
*
*/
CTexture::~CTexture()
{
	if(m_bIsLoaded)
	{
		glDeleteTextures(1, &m_uiTextureID);
		m_bIsLoaded = false;
	}
}
/**
*
* CTexture class Initialise
*
* @author Christopher Howlett
* @param _pRenderer OpenGLRenderer
* @param _pcFilename texture filename
* @param _uiTextureUnit Texture ID
*
*/
bool 
CTexture::Initialise(COpenGLRenderer* _pRenderer, char* _pcFilename, unsigned int _uiTextureUnit)
{
	bool bResult = LoadFromTarga(_pRenderer, _pcFilename, _uiTextureUnit);
	return bResult;
}
/**
*
* CTexture class LoadFromTarga
*
* @author Christopher Howlett
* @param _pRenderer OpenGLRenderer
* @param _pcFilename texture filename
* @param _uiTextureUnit Texture ID
* @return Returns success
*
*/
bool 
CTexture::LoadFromTarga(COpenGLRenderer* _pRenderer, char* _pcFilename, unsigned int _uiTextureUnit)
{
	bool bResult = false;
	FILE* pFile;
	TTargaTexture tTarga;
	ZeroMemory(&tTarga, sizeof(TTargaTexture));
	int iImageSize = 0;
	unsigned char* pData;
	
	//Read binary file
	bResult = (fopen_s(&pFile, _pcFilename, "rb") == 0);
	//LOAD FAILURE!
	if(!bResult)
	{
		Error(L"Targa texture failed to open!");
	}
	//LOAD SUCCESS
	else
	{
		//Read file header
		fread(&tTarga, sizeof(TTargaTexture), 1, pFile);
		m_iTextureDimensions = static_cast<int>(tTarga.width);
	
		//Read texture data
		iImageSize = m_iTextureDimensions * m_iTextureDimensions * 4;
		pData = new unsigned char[iImageSize];
		if(fread(pData, 1, iImageSize, pFile) != iImageSize)
		{
			std::cout << "WARNING: Texture data not read properly!!" << std::endl;
		}
	
		if(tTarga.bpp != 32)
		{
			std::cout << "WARNING: Texture is not 32Bit!!" << std::endl;
		}
		
		//Close file
		fclose(pFile);
		
		//Setup openGL Texture
		_pRenderer->glActiveTexture(GL_TEXTURE0 + _uiTextureUnit);
		glGenTextures(1, &m_uiTextureID);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureID); //Bind texture to ID
		glTexImage2D(	GL_TEXTURE_2D, 
						0, 
						GL_RGBA, 
						static_cast<int>(tTarga.width), 
						static_cast<int>(tTarga.height),
						0, 
						GL_BGRA,
						GL_UNSIGNED_BYTE,
						pData);
	
		//Set to Wrap texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
		//Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
		//Generate mipmaps
		_pRenderer->glGenerateMipmap(GL_TEXTURE_2D);
	
		printf("Loaded %s %iBit texture (%i x %i)\n", _pcFilename, tTarga.bpp, tTarga.width, tTarga.height);
		m_bIsLoaded = true;
	}
	delete[] pData;
	pData = 0;
	return bResult;
}