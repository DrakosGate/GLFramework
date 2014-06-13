//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   framebufferobject.cpp
//  Description :   Code for Class CFrameBufferObject
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <iostream>

// Local Includes

// This Include
#include "framebufferobject.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CFrameBufferObject constructor
*
* @author Christopher Howlett
*
*/
CFrameBufferObject::CFrameBufferObject()
:	m_uiFrameBuffer(0)
,	m_uiTexture(0)
{

}
/**
*
* CFrameBufferObject destructor
*
* @author Christopher Howlett
*
*/
CFrameBufferObject::~CFrameBufferObject()
{
	//Clean up frame buffer
	if(m_uiFrameBuffer) 
	{
		glDeleteFramebuffers(1, &m_uiFrameBuffer);
	}
	if(m_uiTexture) 
	{
		glDeleteTextures(1, &m_uiTexture);
	}
}
/**
*
* CFrameBufferObject Initialisation
*
* @author Christopher Howlett
* @param _iBufferWidth Buffer Width
* @param _iBufferHeight Buffer height
*
*/
bool 
CFrameBufferObject::Initialise(int _iBufferWidth, int _iBufferHeight)
{
	bool bResult = true;
	printf("Creating Frame Buffer Object %i x %i\n", _iBufferWidth, _iBufferHeight);
	
	glGenFramebuffers(1, &m_uiFrameBuffer);

	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);

	glTexImage2D(	GL_TEXTURE_2D, 
					0, 
					GL_DEPTH_COMPONENT, 
					_iBufferWidth, 
					_iBufferHeight, 
					0, 
					GL_DEPTH_COMPONENT,
					GL_FLOAT, 
					NULL);
	//Setup texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//Bind the frame buffer object
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uiFrameBuffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uiTexture, 0);
	//Specify NOT rendering to a colour buffer
	glDrawBuffer(GL_NONE);

	//Check frame buffer status
	GLenum eStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(eStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		bResult = false;
		printf("Frame buffer Error 0x%x\n", eStatus);
	}
	return bResult;
}
/**
*
* CFrameBufferObject Set this buffer for reading
*
* @author Christopher Howlett
*
*/
void 
CFrameBufferObject::SetToRead(GLenum _eTextureID)
{
	glActiveTexture(_eTextureID);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
}
/**
*
* CFrameBufferObject Set this buffer for writing
*
* @author Christopher Howlett
*
*/
void 
CFrameBufferObject::SetToWrite()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uiFrameBuffer);
}