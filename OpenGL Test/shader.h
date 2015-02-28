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
#include "resource.h"

// Types

// Constants

// Prototypes

class Shader : public Resource
{
public:
	Shader();
	virtual ~Shader();

	bool InitialiseShader( char* _pcVertexShader, char* _pcFragmentShader);
	char* LoadShader(char* _pcFilename);
	void SetShader( );
	void SetShaderMatrix( char* _pcVariableName, const glm::mat4& _pMatrix );
	void SetShaderInteger( char* _pcVariableName, int _iInteger);
	void SetShaderFloat( char* _pcVariableName, float _fFloat);
	void SetShaderVector3( char* _pcVariableName, glm::vec3& _rVector);
	void SetShaderVector4( char* _pcVariableName, glm::vec4& _rVector);
	void OutputShaderErrorMessage( unsigned int _uiShaderId);

	int GetShaderProgram() const;

private:
	Shader(const Shader& _krInstanceToCopy);
	const Shader& operator =(const Shader& _krInstanceToCopy);


private:
	unsigned int m_uiVertexShader;
	unsigned int m_uiFragmentShader;
	unsigned int m_uiShaderProgram;

};

#endif //__SHADER_H__