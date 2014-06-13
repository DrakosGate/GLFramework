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

// Local Includes
#include "openglrenderer.h"
#include "defines.h"

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
CShader::CShader()
:	m_uiVertexShader(0)
,	m_uiFragmentShader(0)
,	m_uiShaderProgram(0)
,	m_pRenderer(0)
{

}
/**
*
* CShader destructor
*
* @author Christopher Howlett
*
*/
CShader::~CShader()
{
	m_pRenderer->glDetachShader(m_uiShaderProgram, m_uiVertexShader);
	m_pRenderer->glDetachShader(m_uiShaderProgram, m_uiFragmentShader);

	//Delete shaders
	m_pRenderer->glDeleteShader(m_uiVertexShader);
	m_pRenderer->glDeleteShader(m_uiFragmentShader);

	//Delete program
	m_pRenderer->glDeleteProgram(m_uiShaderProgram);
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
CShader::InitialiseShader(COpenGLRenderer* _pRenderer, char* _pcVertexShader, char* _pcFragmentShader)
{
	m_pRenderer = _pRenderer;
	bool bSuccess = true;
	int iResult = 0;

	const char* pcVertexShaderBuffer;
	const char* pcFragmentShaderBuffer;
	
	pcVertexShaderBuffer = LoadShader(_pcVertexShader);
	pcFragmentShaderBuffer = LoadShader(_pcFragmentShader);

	//Create vertex and fragment shader objects
	m_uiVertexShader = _pRenderer->glCreateShader(GL_VERTEX_SHADER);
	m_uiFragmentShader = _pRenderer->glCreateShader(GL_FRAGMENT_SHADER);

	//Copy shader strings into shaders
	_pRenderer->glShaderSource(m_uiVertexShader, 1, &pcVertexShaderBuffer, NULL);
	_pRenderer->glShaderSource(m_uiFragmentShader, 1, &pcFragmentShaderBuffer, NULL);

	delete[] pcVertexShaderBuffer;
	delete[] pcFragmentShaderBuffer;
	pcVertexShaderBuffer = 0;
	pcFragmentShaderBuffer = 0;

	//Compile shaders
	_pRenderer->glCompileShader(m_uiVertexShader);
	_pRenderer->glCompileShader(m_uiFragmentShader);
	
	//Check success of initialisation
	_pRenderer->glGetShaderiv(m_uiVertexShader, GL_COMPILE_STATUS, &iResult);
	if(iResult != 1)
	{
		OutputShaderErrorMessage(_pRenderer, m_uiVertexShader);
		Error(L"Vertex shader failed initialisation");
		bSuccess = false;
	}
	_pRenderer->glGetShaderiv(m_uiFragmentShader, GL_COMPILE_STATUS, &iResult);
	if(iResult != 1)
	{
		OutputShaderErrorMessage(_pRenderer, m_uiFragmentShader);
		Error(L"Fragment shader failed initialisation");
		bSuccess = false;
	}

	//Create shader program object
	m_uiShaderProgram = _pRenderer->glCreateProgram();

	//Attach shaders to program object
	_pRenderer->glAttachShader(m_uiShaderProgram, m_uiVertexShader);
	_pRenderer->glAttachShader(m_uiShaderProgram, m_uiFragmentShader);

	//Bind shader input variables
	_pRenderer->glBindAttribLocation(m_uiShaderProgram, 0, "inputPosition");
	_pRenderer->glBindAttribLocation(m_uiShaderProgram, 1, "inputTexCoord");
	_pRenderer->glBindAttribLocation(m_uiShaderProgram, 2, "inputNormal");
	//Link shader program
	_pRenderer->glLinkProgram(m_uiShaderProgram);

	//Check success of link creation
	//_pRenderer->glGetProgramiv(m_uiShaderProgram, GL_LINK_STATUS, &iResult);
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
CShader::LoadShader(char* _pcFilename)
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
CShader::SetShader(COpenGLRenderer* _pRenderer)
{
	_pRenderer->glUseProgram(m_uiShaderProgram);
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
CShader::SetShaderMatrix(COpenGLRenderer* _pRenderer, char* _pcVariableName, float* _pMatrix)
{
	unsigned int uiLocation = _pRenderer->glGetUniformLocation(m_uiShaderProgram, _pcVariableName);
	_pRenderer->glUniformMatrix4fv(uiLocation, 1, false, _pMatrix);
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
CShader::SetShaderInteger(COpenGLRenderer* _pRenderer, char* _pcVariableName, int _iInteger)
{
	unsigned int uiLocation = _pRenderer->glGetUniformLocation(m_uiShaderProgram, _pcVariableName);
	_pRenderer->glUniform1i(uiLocation, _iInteger);
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
CShader::SetShaderFloat(COpenGLRenderer* _pRenderer, char* _pcVariableName, float _fFloat)
{
	unsigned int uiLocation = _pRenderer->glGetUniformLocation(m_uiShaderProgram, _pcVariableName);
	_pRenderer->glUniform1f(uiLocation, _fFloat);
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
CShader::SetShaderVector3(COpenGLRenderer* _pRenderer, char* _pcVariableName, TVector3& _rVector)
{
	unsigned int uiLocation = _pRenderer->glGetUniformLocation(m_uiShaderProgram, _pcVariableName);
	_pRenderer->glUniform3fv(uiLocation, 1, reinterpret_cast<float*>(&_rVector));
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
CShader::SetShaderVector4(COpenGLRenderer* _pRenderer, char* _pcVariableName, TVector4& _rVector)
{
	unsigned int uiLocation = _pRenderer->glGetUniformLocation(m_uiShaderProgram, _pcVariableName);
	_pRenderer->glUniform4fv(uiLocation, 1, reinterpret_cast<float*>(&_rVector));
}
/**
*
* CShader Outputs shader compilation error messages
*
* @author Christopher Howlett
*
*/
void 
CShader::OutputShaderErrorMessage(COpenGLRenderer* _pRenderer, unsigned int _uiShaderId)
{
	int iLogSize = 0;
	char* pcInfoLog;
	ofstream fOutputFile;

	_pRenderer->glGetShaderiv(_uiShaderId, GL_INFO_LOG_LENGTH, &iLogSize);

	// Increment the size by one to handle also the null terminator
	++iLogSize;

	pcInfoLog = new char[iLogSize];
	_pRenderer->glGetShaderInfoLog(_uiShaderId, iLogSize, NULL, pcInfoLog);

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
CShader::GetShaderProgram() const
{
	return m_uiShaderProgram;
}