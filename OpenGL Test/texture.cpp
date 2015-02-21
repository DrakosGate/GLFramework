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
#include <vector>

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
Texture::Texture()
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
Texture::~Texture()
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
Texture::Initialise(COpenGLRenderer* _pRenderer, char* _pcFilename, unsigned int _uiTextureUnit)
{
	bool bResult = true;
	int iFilenameLength = strlen(_pcFilename);
	//TGA
	if (_pcFilename[iFilenameLength - 3] == 't' && _pcFilename[iFilenameLength - 2] == 'g' && _pcFilename[iFilenameLength - 1] == 'a')
	{
		bResult = LoadFromTarga(_pRenderer, _pcFilename, _uiTextureUnit);
	}
	//PNG
	else if (_pcFilename[iFilenameLength - 3] == 'p' && _pcFilename[iFilenameLength - 2] == 'n' && _pcFilename[iFilenameLength - 1] == 'g')
	{
		bResult = LoadFromPNG(_pRenderer, _pcFilename, _uiTextureUnit);
	}
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
Texture::LoadFromPNG(COpenGLRenderer* _pRenderer, char* _pcFilename, unsigned int _uiTextureUnit)
{
	bool bResult = false;
	//
	//std::vector<unsigned char> imageData;
	//unsigned int iWidth;
	//unsigned int iHeight;
	//lodepng::decode(imageData, iWidth, iHeight, _pcFilename);
	//
	////Copy data to a buffer
	//unsigned char* pcBuffer = new unsigned char[imageData.size()];
	//for (unsigned int iChar = 0; iChar < imageData.size(); ++iChar)
	//{
	//	pcBuffer[iChar] = imageData[iChar];
	//}
	//
	////Setup openGL Texture
	//glActiveTexture(GL_TEXTURE0 + _uiTextureUnit);
	//glGenTextures(	1, &m_uiTextureID);
	//glBindTexture(	GL_TEXTURE_2D, m_uiTextureID); //Bind texture to ID
	//glTexImage2D(	GL_TEXTURE_2D,
	//				0,
	//				GL_RGBA,
	//				static_cast<int>(iWidth),
	//				static_cast<int>(iHeight),
	//				0,
	//				GL_BGRA,
	//				GL_UNSIGNED_BYTE,
	//				pcBuffer);
	//
	////Set to Wrap texture
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//
	////Set texture filtering
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//
	////Generate mipmaps
	//glGenerateMipmap(GL_TEXTURE_2D);
	//
	//if (m_uiTextureID == 0)
	//{
	//	printf("Failed to load PNG: %s\n", _pcFilename); 
	//	bResult = false;
	//}
	//else
	//{
	//	printf("Loaded Texture: %s (%i x %i)\n", _pcFilename, iWidth, iHeight);
	//	m_bIsLoaded = true;
	//}
	//delete[] pcBuffer;
	//pcBuffer = 0;

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
Texture::LoadFromTarga(COpenGLRenderer* _pRenderer, char* _pcFilename, unsigned int _uiTextureUnit)
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
		glActiveTexture(GL_TEXTURE0 + _uiTextureUnit);
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
		glGenerateMipmap(GL_TEXTURE_2D);
	
		printf("Loaded %s %iBit texture (%i x %i)\n", _pcFilename, tTarga.bpp, tTarga.width, tTarga.height);
		m_bIsLoaded = true;
	}
	delete[] pData;
	pData = 0;
	return bResult;
}
/**
*
* CTexture class GetTextureID
*
* @author Christopher Howlett
* @return Returns the texture ID
*
*/
unsigned int
Texture::GetTextureID() const
{
	return m_uiTextureID;
}