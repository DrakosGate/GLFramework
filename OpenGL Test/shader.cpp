//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   CShader.cpp
//  Description :   Code for Class CShader
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

// Local Includes
#include "defines.h"
#include "openglrenderer.h"

// This Include
#include "shader.h"

// Static Variables

// Static Function Prototypes

// Implementation
using namespace std;
/**
*
* CShader constructor
*
* @author Christopher Howlett
*
*/
Shader::Shader()
:	m_uiVertexShader(0)
,	m_uiFragmentShader(0)
,	m_uiShaderProgram(0)
{

}
/**
*
* CShader destructor
*
* @author Christopher Howlett
*
*/
Shader::~Shader()
{
	glDetachShader(m_uiShaderProgram, m_uiVertexShader);
	glDetachShader(m_uiShaderProgram, m_uiFragmentShader);

	//Delete shaders
	glDeleteShader(m_uiVertexShader);
	glDeleteShader(m_uiFragmentShader);

	//Delete program
	glDeleteProgram(m_uiShaderProgram);
}
/**
*
* CShader Initialisation
*
* @author Christopher Howlett
* @param _pRenderer OpenGL Renderer
* @param _pcVertexShader Vertex shader filename
* @param _pcFragmentShader Fragment shader filename
*
*/
bool 
Shader::InitialiseShader( char* _pcVertexShader, char* _pcFragmentShader)
{
	bool bSuccess = true;
	int iResult = 0;

	const char* pcVertexShaderBuffer;
	const char* pcFragmentShaderBuffer;
	
	pcVertexShaderBuffer = LoadShader(_pcVertexShader);
	pcFragmentShaderBuffer = LoadShader(_pcFragmentShader);

	//Create vertex and fragment shader objects
	m_uiVertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_uiFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Copy shader strings into shaders
	glShaderSource(m_uiVertexShader, 1, &pcVertexShaderBuffer, NULL);
	glShaderSource(m_uiFragmentShader, 1, &pcFragmentShaderBuffer, NULL);

	delete[] pcVertexShaderBuffer;
	delete[] pcFragmentShaderBuffer;
	pcVertexShaderBuffer = 0;
	pcFragmentShaderBuffer = 0;

	//Compile shaders
	glCompileShader(m_uiVertexShader);
	glCompileShader(m_uiFragmentShader);
	
	//Check success of initialisation
	glGetShaderiv(m_uiVertexShader, GL_COMPILE_STATUS, &iResult);
	if(iResult != 1)
	{
		OutputShaderErrorMessage( m_uiVertexShader);
		Error(L"Vertex shader failed initialisation");
		bSuccess = false;
	}
	glGetShaderiv(m_uiFragmentShader, GL_COMPILE_STATUS, &iResult);
	if(iResult != 1)
	{
		OutputShaderErrorMessage( m_uiFragmentShader);
		Error(L"Fragment shader failed initialisation");
		bSuccess = false;
	}

	//Create shader program object
	m_uiShaderProgram = glCreateProgram();

	//Attach shaders to program object
	glAttachShader(m_uiShaderProgram, m_uiVertexShader);
	glAttachShader(m_uiShaderProgram, m_uiFragmentShader);

	//Bind shader input variables
	glBindAttribLocation(m_uiShaderProgram, 0, "inputPosition");
	glBindAttribLocation(m_uiShaderProgram, 1, "inputTexCoord");
	glBindAttribLocation(m_uiShaderProgram, 2, "inputNormal");
	//Link shader program
	glLinkProgram(m_uiShaderProgram);

	//Check success of link creation
	//glGetProgramiv(m_uiShaderProgram, GL_LINK_STATUS, &iResult);
	if(iResult != 1)
	{
		Error(L"Shader linking failed!!");
		bSuccess = false;
	}

	return bSuccess;
}
/**
*
* CShader Load Shader
*
* @author Christopher Howlett
* @param _pcFilename Filename of shader to load
*
*/
char* 
Shader::LoadShader(char* _pcFilename)
{
	ifstream fileIn;
	char cInput;
	char* pcTextBuffer = 0;
	int iFileSize = 0;

	fileIn.open(_pcFilename);
	if(fileIn.is_open() == false)
	{
		Error(L"Could not open shader file");
	}

	//Check filesize
	fileIn.get(cInput);
	while(!fileIn.eof())
	{
		++iFileSize;
		fileIn.get(cInput);
	}

	fileIn.close();

	//Copy file data into new text buffer
	pcTextBuffer = new char[iFileSize + 1];
	fileIn.open(_pcFilename);
	fileIn.read(pcTextBuffer, iFileSize);

	fileIn.close();
	pcTextBuffer[iFileSize] = '\0';

	return pcTextBuffer;
}
/**
*
* CShader Set this shader
*
* @author Christopher Howlett
*
*/
void 
Shader::SetShader( )
{
	glUseProgram(m_uiShaderProgram);
}
/**
*
* CShader SetShaderMatrix
*
* @author Christopher Howlett
* @param _pMatrix ShaderMatrix variable
*
*/
void 
Shader::SetShaderMatrix( char* _pcVariableName, const glm::mat4& _pMatrix)
{
	unsigned int uiLocation = glGetUniformLocation(m_uiShaderProgram, _pcVariableName);
	glUniformMatrix4fv(uiLocation, 1, false, glm::value_ptr(_pMatrix) );
}
/**
*
* CShader SetShaderInteger
*
* @author Christopher Howlett
* @param _iInteger Shader Integer variable
*
*/
void 
Shader::SetShaderInteger( char* _pcVariableName, int _iInteger)
{
	unsigned int uiLocation = glGetUniformLocation(m_uiShaderProgram, _pcVariableName);
	glUniform1i(uiLocation, _iInteger);
}
/**
*
* CShader SetShaderFloat
*
* @author Christopher Howlett
* @param _fFloat Shader Float variable
*
*/
void 
Shader::SetShaderFloat( char* _pcVariableName, float _fFloat)
{
	unsigned int uiLocation = glGetUniformLocation(m_uiShaderProgram, _pcVariableName);
	glUniform1f(uiLocation, _fFloat);
}
/**
*
* CShader SetShaderVector3
*
* @author Christopher Howlett
* @param _rVector Shader Vector3 variable
*
*/
void 
Shader::SetShaderVector3( char* _pcVariableName, glm::vec3& _rVector)
{
	unsigned int uiLocation = glGetUniformLocation(m_uiShaderProgram, _pcVariableName);
	glUniform3fv( uiLocation, 1, glm::value_ptr( _rVector ) );
}
/**
*
* CShader SetShaderVector4
*
* @author Christopher Howlett
* @param _rVector Shader Vector4 variable
*
*/
void 
Shader::SetShaderVector4( char* _pcVariableName, glm::vec4& _rVector)
{
	unsigned int uiLocation = glGetUniformLocation(m_uiShaderProgram, _pcVariableName);
	glUniform4fv( uiLocation, 1, glm::value_ptr( _rVector ) );
}
/**
*
* CShader Outputs shader compilation error messages
*
* @author Christopher Howlett
*
*/
void 
Shader::OutputShaderErrorMessage( unsigned int _uiShaderId)
{
	int iLogSize = 0;
	char* pcInfoLog;
	ofstream fOutputFile;

	glGetShaderiv(_uiShaderId, GL_INFO_LOG_LENGTH, &iLogSize);

	// Increment the size by one to handle also the null terminator
	++iLogSize;

	pcInfoLog = new char[iLogSize];
	glGetShaderInfoLog(_uiShaderId, iLogSize, NULL, pcInfoLog);

	// Open a error log file
	fOutputFile.open("shader-error.txt");

	// Write out error message
	for(int i = 0; i < iLogSize; ++i)
	{
		fOutputFile << pcInfoLog[i];
	}

	// Close log file
	fOutputFile.close();

	delete[] pcInfoLog;
	pcInfoLog = 0;

	return;
}
/**
*
* CShader Returns ID of this shader program
*
* @author Christopher Howlett
* @return Returns program ID
*
*/
int 
Shader::GetShaderProgram() const
{
	return m_uiShaderProgram;
}