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
#include "level.h"

// Types
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092

//Typedefines - used for OpenGL 4.0
typedef HGLRC(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC _hdc, HGLRC _hShared, const int* _pAttribList);

// Constants

// Prototypes
class Camera;
class Clock;
class Window;
class ConsoleWindow;

class OpenGLRenderer : public IRenderer
{
	//Member functions
public:
	OpenGLRenderer();
	virtual ~OpenGLRenderer();

	virtual bool Initialise( Window* _pWindow, int _iWindowWidth, int _iWindowHeight, TInputStruct* _pInput) override;
	virtual bool InitialiseExtensions(HWND _hWnd) override;
	virtual void SetupOpenGL(HWND _hWnd, bool _bVSync);
	virtual void CleanUp();
	virtual void ExecuteOneFrame(float _fDeltaTick);

	virtual void PreDraw();
	virtual void Draw(const Camera* _pCurrentCamera);
	virtual void PostDraw();
	virtual void Process( const float _fDeltaTime );
	virtual void ProcessInput();
	
	//virtual void CreateEntities();
	//virtual void CreateModels();
	//virtual void LoadShaders();


	//void DrawLine(glm::vec3& _rStart, glm::vec3& _rEnd, glm::vec3& _rColour);
	//
	//void SendLightData();
	//void LoadTextures();
	//void CreateLights();

private:
	bool LoadExtensions();

//Member variables
private:
	Level m_pLevel;

	//Input
	TInputStruct* m_pInput;

	//Debugging
	ConsoleWindow* m_pConsole;
	float m_fGameTime;

	HWND m_hWindow;
	HDC m_deviceContext;
	HGLRC m_renderingContext;

	char m_videocardInfo[128];

};

#endif //OPENGLRENDERER