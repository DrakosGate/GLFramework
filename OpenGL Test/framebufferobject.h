//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   framebufferobject.h
//  Description :   Class of CFrameBufferObject
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __FRAMEBUFFEROBJECT_H__
#define __FRAMEBUFFEROBJECT_H__

// Library Includes

// Local Includes
#include "openglrenderer.h"

// Types

// Constants

// Prototypes

class CFrameBufferObject
{
public:
	CFrameBufferObject();
	virtual ~CFrameBufferObject();

	virtual bool Initialise(int _iBufferWidth, int _iBufferHeight);
	virtual void SetToRead(GLenum _eTextureID);
	virtual void SetToWrite();

private:
	CFrameBufferObject(const CFrameBufferObject& _kr);
	CFrameBufferObject& operator= (const CFrameBufferObject& _rhs);
	
protected:
	GLuint m_uiFrameBuffer;
	GLuint m_uiTexture;
};

#endif //__FRAMEBUFFEROBJECT_H__