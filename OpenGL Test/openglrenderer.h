//  File Name   :   COpenGLRenderer.h
//  Description :   Class of COpenGLRenderer
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#ifndef __OPENGLRENDERER_H__
#define __OPENGLRENDERER_H__


// Library Includes
#include <Windows.h>
#include <math.h>
#include <vld.h>
#include <GL\glew.h>
#include <gl\GL.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

// Local Includes
#include "chrismaths.h"
#include "irenderer.h"
#include "opengldefinitions.h"

// Types

// Constants

// Prototypes
class CCamera;
class CModel;
class CShader;
class CClock;
class CTexture;
class CConsoleWindow;
class CLightManager;
class CTerrain;
class CFrameBufferObject;
class CHelicopter;
class CShip;

class COpenGLRenderer : public IRenderer
{
	//Member functions
public:
	COpenGLRenderer();
	virtual ~COpenGLRenderer();

	virtual bool Initialise(HWND _hWnd, int _iWindowWidth, int _iWindowHeight, TInputStruct* _pInput);
	
	virtual void PreDraw();
	virtual void Draw(CCamera* _pCurrentCamera);
	virtual void PostDraw();
	virtual void Process(float _fDeltaTick);
	virtual void ProcessInput();
	
	virtual void SetupOpenGL(HWND _hWnd);
	virtual void CreateEntities();
	virtual void CreateModels();
	virtual void LoadShaders();
	virtual void CleanUp();

	virtual void ExecuteOneFrame(float _fDeltaTick);

	void DrawLine(TVector3& _rStart, TVector3& _rEnd, TVector3& _rColour);

	void SendLightData();
	void LoadTextures();
	void CreateLights();
	void LoadExtensions();

//Member variables
private:
	//Scene objects
	CHelicopter* m_pHelicopter;
	CShip* m_pShip;

	//Models
	CModel* m_pModelCollection;
	CModel* m_pScreenSurface;
	CTerrain* m_pTerrain;

	//Shaders
	CShader* m_pColourShader;

	//Cameras
	CCamera* m_pPerspectiveCamera;
	CCamera* m_pOrthographicCamera;
	bool m_bIsFirstPerson;

	//Input
	TInputStruct* m_pInput;

	//Textures
	CTexture* m_pTextureCollection;

	//Lights
	CLightManager* m_pLightManager;
	TVector4 m_vecAmbientLight;

	//Frame Buffers
	CFrameBufferObject* m_pFrameBuffer;

	//GLEW

	//Debugging
	CConsoleWindow* m_pConsole;
	float m_fGameTime;

	HWND m_hWindow;
	HDC m_hDevContext;
	HGLRC m_hRenderContext;

	//Matrices
	TMatrix m_matWorld;

	//Camera stuff
	float m_fFOV;
	float m_fAspectRatio;

	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

public:
	//OpenGL Function pointers
	PFNGLATTACHSHADERPROC glAttachShader;
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	PFNGLDELETEPROGRAMPROC glDeleteProgram;
	PFNGLDELETESHADERPROC glDeleteShader;
	PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
	PFNGLDETACHSHADERPROC glDetachShader;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	PFNGLGETPROGRAMIVPROC glGetProgramiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLGETSHADERIVPROC glGetShaderiv;
	PFNGLLINKPROGRAMPROC glLinkProgram;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	PFNGLACTIVETEXTUREPROC glActiveTexture;
	PFNGLUNIFORM1IPROC glUniform1i;
	PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
	PFNGLUNIFORM3FVPROC glUniform3fv;
	PFNGLUNIFORM4FVPROC glUniform4fv;
	PFNGLUNIFORM1FPROC glUniform1f;
};

#endif //OPENGLRENDERER