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
#include "resource.h"

// Types

// Constants

// Prototypes
class OpenGLRenderer;

class Texture : public Resource
{
//Member functions
public:
	Texture();
	virtual ~Texture();

	bool Initialise( const char* _pcFilename, unsigned int _uiTextureUnit);
	bool LoadFromPNG( const char* _pcFilename, unsigned int _uiTextureUnit);
	bool LoadFromTarga( const char* _pcFilename, unsigned int _uiTextureUnit);
	
private:
	Texture(const Texture& _krInstanceToCopy);
	const Texture& operator =(const Texture& _krInstanceToCopy);

//Member variables
private:
	bool m_bIsLoaded;
	int m_iTextureDimensions;

};

#endif //__TEXTURE_H__
