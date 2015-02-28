//  File Name   :   COpenGLRenderer.cpp
//  Description :   Code for Class COpenGLRenderer
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <math.h>

// Local Includes
#include "defines.h"
#include "camera.h"
#include "clock.h"
#include "consolewindow.h"
#include "level.h"
#include "window.h"

// This Include
#include "openglrenderer.h"

// Static Variables

// Static Function Prototypes

// Implementation

OpenGLRenderer::OpenGLRenderer()
: m_hWindow(0)
, m_deviceContext(0)
, m_pInput(0)
, m_pConsole(0)
{

}
OpenGLRenderer::~OpenGLRenderer()
{
	if(m_pConsole)
	{
		//Shutdown console
		m_pConsole->ShutdownConsole();

		delete m_pConsole;
		m_pConsole = 0;
	}

	//Shutdown OpenGL
	CleanUp();
}
bool 
OpenGLRenderer::Initialise( Window* _pWindow, int _iWindowWidth, int _iWindowHeight, TInputStruct* _pInput )
{
	m_hWindow = _pWindow->GetHandle();
	m_pInput = _pInput;
	m_iWindowWidth = _iWindowWidth;
	m_iWindowHeight = _iWindowHeight;

	//Start up console
	m_pConsole = new ConsoleWindow();
	m_pConsole->InitialiseConsole();
	
	//Initialise OpenGL 
	InitialiseExtensions( m_hWindow );
	SetupOpenGL( m_hWindow, false );

	//Initialise camera objects
	m_pLevel.Initialise( this, _pWindow );

	
	return true;
}
void
OpenGLRenderer::SetupOpenGL( HWND _hWnd, bool _bVSync )
{
	bool bResult = false;
	int iResult = 0;
	printf("Initialising OpenGL\n");

	PIXELFORMATDESCRIPTOR pixelFormatDesc;
	int pixelFormat;
	int attributeList[5];
	char* pVendorString;
	char* pRendererString;

	m_deviceContext = GetDC(m_hWindow);
	ErrAssert(m_deviceContext, L"Could not create device context");

	pixelFormat = ChoosePixelFormat(m_deviceContext, &pixelFormatDesc);
	ErrAssert(pixelFormat, L"Could not get pixel format");
	bResult = SetPixelFormat(m_deviceContext, pixelFormat, &pixelFormatDesc) == 0;
	DWORD errorCode = GetLastError();
	ErrAssert(bResult, L"Could not set pixel format");

	//Temp 2.1 context
	HGLRC tempContext = wglCreateContext(m_deviceContext);
	wglMakeCurrent(m_deviceContext, tempContext); //Temp 2.1 context

	printf("- Initialising GLEW\n");
	GLenum result = glewInit();
	ErrAssert(result == GLEW_OK, L"Could not initialise GLEW");

	//Set version to 4.0
	attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attributeList[1] = 4;
	attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attributeList[3] = 0;
	attributeList[4] = 0;

	//Create opengl 4.0 rendering context
	m_renderingContext = wglCreateContextAttribsARB(m_deviceContext, 0, attributeList);
	if (m_renderingContext)
	{
		printf("== OpenGL 4.0 is supported ==\n");
		//Replace current with 4.0
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);
		bResult = wglMakeCurrent(m_deviceContext, m_renderingContext) == TRUE;
		ErrAssert(bResult, L"Could not set 4.0 context");
	}

	pVendorString = (char*)glGetString(GL_VENDOR);
	pRendererString = (char*)glGetString(GL_RENDERER);
	strcpy_s(m_videocardInfo, pVendorString);
	strcat_s(m_videocardInfo, "-");
	strcat_s(m_videocardInfo, pRendererString);

	//Vsync
	bResult = wglSwapIntervalEXT(_bVSync) == TRUE;
	ErrAssert(bResult, L"Could not set VSync varaible");

	int glVersion[2] = { -1, -1 };
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
	printf("OpenGL Initialise Success: [%i.%i]\n", glVersion[0], glVersion[1]);
	printf("GPU: %s\n", m_videocardInfo); 

	//Setup OpenGL for Rendering
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void
OpenGLRenderer::CleanUp()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_renderingContext);
	ReleaseDC(m_hWindow, m_deviceContext);
}

void
OpenGLRenderer::ExecuteOneFrame(float _fDeltaTick)
{
	//Process calculations
	Process(_fDeltaTick);

	//Render scene to screen
	PreDraw();

	//Render using perspective cam
	Draw( m_pLevel.GetPerspectiveCamera() );

	//Finish, swap buffers and present scene
	PostDraw();
}
void 
OpenGLRenderer::PreDraw()
{
	glBegin(GL_TRIANGLES);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	//Send shader information
	//m_pColourShader->SetShader(this);
	//Lighting variables
	//SendLightData();
}
void 
OpenGLRenderer::Draw(const Camera* _pCurrentCamera)
{	
	m_pLevel.Draw( );
}
void 
OpenGLRenderer::PostDraw()
{
	glFlush();
	SwapBuffers(m_deviceContext);
	glFinish();
}
void 
OpenGLRenderer::Process( const float _fDeltaTime )
{
	ProcessInput();

	m_fGameTime += _fDeltaTime;
	//Process everything
	m_pLevel.Process( _fDeltaTime );
}
void 
OpenGLRenderer::ProcessInput()
{
	if(m_pInput->bSpace)
	{
		
	}
}
/*
void 
COpenGLRenderer::CreateEntities()
{
	//Initialise camera objects
	m_pPerspectiveCamera = new Camera();
	m_pPerspectiveCamera->Initialise(m_fFOV, m_fAspectRatio, 0.01f, 1000.0f, 100.0f, 50.0f, m_iWindowWidth, m_iWindowHeight, CAMERA_PERSPECTIVE);
	m_pPerspectiveCamera->SetInput(m_pInput);
	m_pPerspectiveCamera->SetPosition(glm::vec3(0.0f, 2.0f, -5.0f));
	m_pOrthographiCamera = new Camera();
	m_pOrthographiCamera->Initialise(m_fFOV, m_fAspectRatio, 0.1f, 1000.0f, 5.0f, 2.0f, m_iWindowWidth, m_iWindowHeight, CAMERA_ORTHOGONAL);
	m_pOrthographiCamera->SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
	m_pOrthographiCamera->SetForward(glm::vec3(-1.0f, 0.0f, 0.0f));

	//Initialise models
	CreateModels();

	m_pScreenSurface = new CModel();
	m_pScreenSurface->Initialise(this, glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, TEXTURE_SQUARES, m_pColourShader);
	m_pScreenSurface->LoadSquare();

	m_pTerrain = new CTerrain();
	m_pTerrain->Initialise(this, glm::vec3(0.0f, 0.0f, 0.0f), 100.0f, TEXTURE_FLOORTILE, m_pColourShader);
	m_pTerrain->LoadTerrain(10, 50.0f);
	m_pTerrain->SetNormalMap(TEXTURE_FLOORNORMAL);

	//m_pHelicopter = new CHelicopter();
	//m_pHelicopter->Initialise(&m_pModelCollection[MODEL_HELICHASSIS], &m_pModelCollection[MODEL_HELIROTOR], m_pInput);
	//m_pHelicopter->SetPosition(glm::vec3(0.0f, 1.0f, -10.0f));
	//
	////m_pPerspectiveCamera->SetParent(m_pHelicopter, &glm::vec3(0.0f, 0.8f, -4.0f)); //Third person
	////m_pPerspectiveCamera->SetParent(m_pHelicopter, &glm::vec3(0.0f, 0.2f, 0.3f)); //First person
	//m_pShip = new CShip();
	//m_pShip->Initialise(&m_pModelCollection[MODEL_SHIP], m_pInput);
	//m_pShip->SetPosition(glm::vec3(0.0f, 2.0f, -10.0f));
	//m_pLightManager->GetSpot(0)->SetParent(m_pShip, &glm::vec3(0.0f, 0.0f, 1.0f));
	//m_pPerspectiveCamera->SetParent(m_pShip, &glm::vec3(0.0f, 0.8f, -4.0f)); //Third person
	//m_pPerspectiveCamera->SetParent(m_pShip, &glm::vec3(0.0f, 0.0f, 0.2f)); //First person

	//Create Frame Buffer Objects
	//m_pFrameBuffer = new CFrameBufferObject();
	//m_pFrameBuffer->Initialise(m_iWindowWidth, m_iWindowHeight);
	
}
void
COpenGLRenderer::CreateModels()
{
	//Initialise all models used in this scene
	//m_pModelCollection = new CModel[MODEL_MAX];
	//
	//m_pModelCollection[MODEL_LOOP].Initialise(this, glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, TEXTURE_STONE, m_pColourShader);
	//m_pModelCollection[MODEL_LOOP].LoadFromOBJ("Assets/checkerLoop.obj");
	//m_pModelCollection[MODEL_LOOP].SetNormalMap(TEXTURE_STONENORMAL);
	//m_pModelCollection[MODEL_LOOP].SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
	//
	//m_pModelCollection[MODEL_HELICHASSIS].Initialise(this, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_STONE, m_pColourShader);
	//m_pModelCollection[MODEL_HELICHASSIS].LoadFromOBJ("Assets/helicopter.obj");
	//m_pModelCollection[MODEL_HELICHASSIS].SetNormalMap(TEXTURE_STONENORMAL);
	//
	//m_pModelCollection[MODEL_HELIROTOR].Initialise(this, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_STONE, m_pColourShader);
	//m_pModelCollection[MODEL_HELIROTOR].LoadFromOBJ("Assets/rotor.obj");
	//m_pModelCollection[MODEL_HELIROTOR].SetNormalMap(TEXTURE_STONENORMAL);
	//
	//m_pModelCollection[MODEL_SHIP].Initialise(this, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_SHIP, m_pColourShader);
	//m_pModelCollection[MODEL_SHIP].LoadFromOBJ("Assets/ship.obj");
	//m_pModelCollection[MODEL_SHIP].SetNormalMap(TEXTURE_SHIPNORMAL);
	//
	//m_pModelCollection[MODEL_CUBE].Initialise(this, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_STONE, m_pColourShader);
	//m_pModelCollection[MODEL_CUBE].LoadFromOBJ("Assets/cube.obj");
	//m_pModelCollection[MODEL_CUBE].SetNormalMap(TEXTURE_STONENORMAL);
	
}
void 
COpenGLRenderer::LoadShaders()
{
	m_pColourShader = new CShader();
	m_pColourShader->InitialiseShader(this, "Shaders/diffuselighting.vert", "Shaders/diffuselighting.frag");
}
//Draw debug line to window
void 
COpenGLRenderer::DrawLine(glm::vec3& _rStart, glm::vec3& _rEnd, glm::vec3& _rColour)
{
	glBegin(GL_LINES);
	glLineWidth(5.5); 
	glColor3f(_rColour.x, _rColour.y, _rColour.z);
	glVertex3f(_rStart.x, _rStart.y, _rStart.z);
	glVertex3f(_rEnd.x, _rEnd.y, _rEnd.z);
	glEnd();
}
void 
COpenGLRenderer::SendLightData()
{
	//DIRECTIONAL LIGHTING
	CDirectionalLight* pDirectionalLight = m_pLightManager->GetDirectional(0);
	m_pColourShader->SetShaderVector3(this, "gDirectionalLight.vecDirection", pDirectionalLight->GetDirection());
	m_pColourShader->SetShaderVector4(this, "gDirectionalLight.tBaseLight.gLightColour", pDirectionalLight->GetColour());
	m_pColourShader->SetShaderVector4(this, "gAmbientLight", m_vecAmbientLight);
	m_pColourShader->SetShaderFloat(this, "gDirectionalLight.tBaseLight.gSpecularPower", pDirectionalLight->GetSpecularPower());

	//POINT LIGHTING 
	CPointLight* pThisPoint;
	char cShaderVar[64];
	int iNumPoints = m_pLightManager->GetLightCount(LIGHT_POINT);
	m_pColourShader->SetShaderInteger(this, "gNumPointLights", iNumPoints);
	for(int iPoint = 0; iPoint < iNumPoints; ++iPoint)
	{
		pThisPoint = m_pLightManager->GetPoint(iPoint);
		sprintf_s(cShaderVar, "gPointLight[%i].tBaseLight.gLightColour", iPoint);		//COLOUR
		m_pColourShader->SetShaderVector4(this, cShaderVar, pThisPoint->GetColour());
		sprintf_s(cShaderVar, "gPointLight[%i].tBaseLight.gSpecularPower", iPoint);		//SPECULAR 
		m_pColourShader->SetShaderFloat(this, cShaderVar, pThisPoint->GetSpecularPower());
		sprintf_s(cShaderVar, "gPointLight[%i].vecPosition", iPoint);					//POSITION
		m_pColourShader->SetShaderVector3(this, cShaderVar, pThisPoint->GetPosition());
		sprintf_s(cShaderVar, "gPointLight[%i].vecAttenuation", iPoint);				//ATTEN
		m_pColourShader->SetShaderVector3(this, cShaderVar, pThisPoint->GetAttenuation());
	}
	//SPOT LIGHTS 
	CSpotLight* pThisSpot;
	int iNumSpots = m_pLightManager->GetLightCount(LIGHT_SPOT);
	m_pColourShader->SetShaderInteger(this, "gNumSpotLights", iNumSpots);
	for(int iSpot = 0; iSpot < iNumSpots; ++iSpot)
	{
		pThisSpot = m_pLightManager->GetSpot(iSpot);
		sprintf_s(cShaderVar, "gSpotLight[%i].tBaseLight.gLightColour", iSpot);		//COLOUR
		m_pColourShader->SetShaderVector4(this, cShaderVar, pThisSpot->GetColour());
		sprintf_s(cShaderVar, "gSpotLight[%i].vecDirection", iSpot);				//DIRECTION
		m_pColourShader->SetShaderVector3(this, cShaderVar, pThisSpot->GetDirection());
		sprintf_s(cShaderVar, "gSpotLight[%i].tBaseLight.gSpecularPower", iSpot);	//SPECULAR 
		m_pColourShader->SetShaderFloat(this, cShaderVar, pThisSpot->GetSpecularPower());
		sprintf_s(cShaderVar, "gSpotLight[%i].vecPosition", iSpot);					//POSITION
		m_pColourShader->SetShaderVector3(this, cShaderVar, pThisSpot->GetPosition());
		sprintf_s(cShaderVar, "gSpotLight[%i].vecAttenuation", iSpot);				//ATTEN
		m_pColourShader->SetShaderVector3(this, cShaderVar, pThisSpot->GetAttenuation());
		sprintf_s(cShaderVar, "gSpotLight[%i].fCutoff", iSpot);						//CUTOFF
		m_pColourShader->SetShaderFloat(this, cShaderVar, pThisSpot->GetCutoff());
	}
}
void 
COpenGLRenderer::LoadTextures()
{
	m_pTextureCollection = new CTexture[TEXTURE_MAX];
	m_pTextureCollection[TEXTURE_STONE].Initialise(this, "Assets/stone.tga", TEXTURE_STONE);
	m_pTextureCollection[TEXTURE_STONENORMAL].Initialise(this, "Assets/stoneNormals.tga", TEXTURE_STONENORMAL);
	m_pTextureCollection[TEXTURE_SQUARES].Initialise(this, "Assets/test.tga", TEXTURE_SQUARES);
	m_pTextureCollection[TEXTURE_FLOORTILE].Initialise(this, "Assets/tileDiffuse.tga", TEXTURE_FLOORTILE);
	m_pTextureCollection[TEXTURE_FLOORNORMAL].Initialise(this, "Assets/tileNormal.tga", TEXTURE_FLOORNORMAL);
	m_pTextureCollection[TEXTURE_SHIP].Initialise(this, "Assets/shipTexture.tga", TEXTURE_SHIP);
	m_pTextureCollection[TEXTURE_SHIPNORMAL].Initialise(this, "Assets/shipNormal.tga", TEXTURE_SHIPNORMAL);
}
void 
COpenGLRenderer::CreateLights()
{
	//Ambient light
	m_vecAmbientLight = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	//Scene lights
	m_pLightManager = new CLightManager();
	m_pLightManager->Initialise();
	
	//Directional
	m_pLightManager->AddDirectional(glm::vec3(1.0f, -0.1f, 0.0f), glm::vec4(0.5f, 0.5f, 0.55f, 1.0f), 500.0f);
	//Point
	m_pLightManager->AddPoint(glm::vec3(-5.0f, 5.0f, 0.0f), glm::vec4(1.0f, 0.2f, 0.2f, 1.0f), glm::vec3(1.0f, 0.5f, 0.1f), 50.1f);
	m_pLightManager->AddPoint(glm::vec3(5.0f, 5.0f, 0.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec3(1.0f, 0.5f, 0.1f), 50.1f);
	//Spot
	m_pLightManager->AddSpot(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec3(1.0f, 0.5f, 0.2f), 0.6f, 500.0f);
	//m_pLightManager->AddSpot(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 5.0f, 5.0f) * -1.0f, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec3(1.0f, 0.5f, 0.2f), 0.5f, 50.0f);
}*/

bool
OpenGLRenderer::InitialiseExtensions(HWND _hWnd)
{
	printf("- Initialising OpenGL Extensions\n");
	bool bResult = false;

	int iError = 0;
	HDC deviceContext;
	PIXELFORMATDESCRIPTOR pixelFormat;
	HGLRC renderContext;

	//Get Device context
	deviceContext = GetDC(_hWnd);
	ErrAssert(deviceContext, L"Device context failed");

	//Set default format
	iError = SetPixelFormat(deviceContext, 1, &pixelFormat);
	ErrAssert(iError == 1, L"Invalid pixel format");

	//Create render context
	renderContext = wglCreateContext(deviceContext);
	ErrAssert(renderContext, L"Could not create render context");

	iError = wglMakeCurrent(deviceContext, renderContext);
	ErrAssert(iError == 1, L"Could not make current context");

	//Load OpenGL Extensions
	bResult = LoadExtensions();
	ErrAssert(bResult, L"Could not load extensions");

	//Release
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(renderContext);
	ReleaseDC(m_hWindow, deviceContext);

	return bResult;
}

bool 
OpenGLRenderer::LoadExtensions()
{
	printf("- Loading OpenGL Extensions\n");
	bool bResult = true;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	ErrAssert(wglCreateContextAttribsARB, L"Could not load extensions");

	return bResult;
}