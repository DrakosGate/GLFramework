//  File Name   :   COpenGLRenderer.h
//  Description :   Class of COpenGLRenderer
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com


#ifndef __OPENGLRENDERER_H__
#define __OPENGLRENDERER_H__


// Library Includes
#include <Windows.h>
#include <vld.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <gl/GL.h>
#include <glm/glm.hpp>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

// Local Includes
#include "irenderer.h"

// Types
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092

//Typedefines - used for OpenGL 4.0
typedef HGLRC(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC _hdc, HGLRC _hShared, const int* _pAttribList);

// Constants

// Prototypes
class CCamera;
class CClock;
class CConsoleWindow;

class COpenGLRenderer : public IRenderer
{
	//Member functions
public:
	COpenGLRenderer();
	virtual ~COpenGLRenderer();

	virtual bool Initialise(HWND _hWnd, int _iWindowWidth, int _iWindowHeight, TInputStruct* _pInput);
	virtual bool InitialiseExtensions(HWND _hWnd);
	virtual void SetupOpenGL(HWND _hWnd, bool _bVSync);
	virtual void CleanUp();
	virtual void ExecuteOneFrame(float _fDeltaTick);

	virtual void PreDraw();
	virtual void Draw(CCamera* _pCurrentCamera);
	virtual void PostDraw();
	virtual void Process(float _fDeltaTick);
	virtual void ProcessInput();
	
	//virtual void CreateEntities();
	//virtual void CreateModels();
	//virtual void LoadShaders();


	//void DrawLine(TVector3& _rStart, TVector3& _rEnd, TVector3& _rColour);
	//
	//void SendLightData();
	//void LoadTextures();
	//void CreateLights();

private:
	bool LoadExtensions();

//Member variables
private:
	//Cameras
	CCamera* m_pPerspectiveCamera;
	CCamera* m_pOrthographicCamera;
	bool m_bIsFirstPerson;

	//Input
	TInputStruct* m_pInput;

	//Debugging
	CConsoleWindow* m_pConsole;
	float m_fGameTime;

	HWND m_hWindow;
	HDC m_deviceContext;
	HGLRC m_renderingContext;

	char m_videocardInfo[128];

};

#endif //OPENGLRENDERER