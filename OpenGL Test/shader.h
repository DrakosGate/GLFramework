//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   shader.h
//  Description :   Class of CShader
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __SHADER_H__
#define __SHADER_H__

// Library Includes

// Local Includes
#include "chrismaths.h"

// Types

// Constants

// Prototypes
class COpenGLRenderer;

class CShader
{
public:
	CShader();
	~CShader();

	bool InitialiseShader(COpenGLRenderer* _pRenderer, char* _pcVertexShader, char* _pcFragmentShader);
	char* LoadShader(char* _pcFilename);
	void SetShader(COpenGLRenderer* _pRenderer);
	void SetShaderMatrix(COpenGLRenderer* _pRenderer, char* _pcVariableName, float* _pMatrix);
	void SetShaderInteger(COpenGLRenderer* _pRenderer, char* _pcVariableName, int _iInteger);
	void SetShaderFloat(COpenGLRenderer* _pRenderer, char* _pcVariableName, float _fFloat);
	void SetShaderVector3(COpenGLRenderer* _pRenderer, char* _pcVariableName, TVector3& _rVector);
	void SetShaderVector4(COpenGLRenderer* _pRenderer, char* _pcVariableName, TVector4& _rVector);
	void OutputShaderErrorMessage(COpenGLRenderer* _pRenderer, unsigned int _uiShaderId);

	int GetShaderProgram() const;

private:
	CShader(const CShader& _krInstanceToCopy);
	const CShader& operator =(const CShader& _krInstanceToCopy);


private:
	COpenGLRenderer* m_pRenderer;
	unsigned int m_uiVertexShader;
	unsigned int m_uiFragmentShader;
	unsigned int m_uiShaderProgram;

};

#endif //__SHADER_H__