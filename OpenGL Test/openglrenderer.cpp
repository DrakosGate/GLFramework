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
#include "shader.h"
#include "model.h"
#include "clock.h"
#include "texture.h"
#include "consolewindow.h"
#include "lightmanager.h"
#include "terrain.h"
#include "spotlight.h"
#include "pointlight.h"
#include "directionallight.h"
#include "framebufferobject.h"
#include "helicopter.h"
#include "ship.h"
#include "scenehierarchy.h"
#include "resourcemanager.h"

// This Include
#include "openglrenderer.h"

// Static Variables

// Static Function Prototypes

// Implementation

COpenGLRenderer::COpenGLRenderer()
: m_hWindow(0)
, m_hDevContext(0)
, m_fFOV(0)
, m_fAspectRatio(0)
, m_pPerspectiveCamera(0)
, m_pModelCollection(0)
, m_pColourShader(0)
, m_pInput(0)
, m_pTextureCollection(0)
, m_pConsole(0)
, m_pLightManager(0)
, m_pTerrain(0)
, m_fGameTime(0.0f)
, m_pFrameBuffer(0)
, m_pHelicopter(0)
, m_pShip(0)
, m_pSceneHierarchy(0)
, m_pResourceManager(0)
{

}
COpenGLRenderer::~COpenGLRenderer()
{
	if (m_pResourceManager)
	{
		delete m_pResourceManager;
		m_pResourceManager = 0;
	}
	if (m_pSceneHierarchy)
	{
		delete m_pSceneHierarchy;
		m_pSceneHierarchy = 0;
	}
	//Delete frame buffers
	if(m_pFrameBuffer)
	{
		delete m_pFrameBuffer;
		m_pFrameBuffer = 0;
	}
	if(m_pScreenSurface)
	{
		delete m_pScreenSurface;
		m_pScreenSurface = 0;
	}
	//Delete game objects
	if(m_pShip)
	{
		delete m_pShip;
		m_pShip = 0;
	}
	if(m_pHelicopter)
	{
		delete m_pHelicopter;
		m_pHelicopter = 0;
	}
	if(m_pModelCollection)
	{
		delete[] m_pModelCollection;
		m_pModelCollection = 0;
	}
	if(m_pPerspectiveCamera)
	{
		delete m_pPerspectiveCamera;
		m_pPerspectiveCamera = 0;
	}
	if(m_pOrthographicCamera)
	{
		delete m_pOrthographicCamera;
		m_pOrthographicCamera = 0;
	}
	if(m_pTextureCollection)
	{
		delete[] m_pTextureCollection;
		m_pTextureCollection = 0;
	}
	if(m_pConsole)
	{
		//Shutdown console
		m_pConsole->ShutdownConsole();

		delete m_pConsole;
		m_pConsole = 0;
	}
	if(m_pLightManager)
	{
		delete m_pLightManager;
		m_pLightManager = 0;
	}
	if(m_pTerrain)
	{
		delete m_pTerrain;
		m_pTerrain = 0;
	}
	//Clear shaders
	if(m_pColourShader)
	{
		delete m_pColourShader;
		m_pColourShader = 0;
	}

	//Shutdown OpenGL
	CleanUp();
}
bool 
COpenGLRenderer::Initialise(HWND _hWnd, int _iWindowWidth, int _iWindowHeight, TInputStruct* _pInput)
{
	//Initialise GLEW for additional OpenGL functionality
	glewInit();
	
	m_hWindow = _hWnd;
	m_pInput = _pInput;
	m_iWindowWidth = _iWindowWidth;
	m_iWindowHeight = _iWindowHeight;

	//Start up console
	m_pConsole = new CConsoleWindow();
	m_pConsole->InitialiseConsole();
	
	//Initialise OpenGL 
	SetupOpenGL(_hWnd);

	//Interpret level editor data
	m_pSceneHierarchy = new CSceneHierarchy();
	m_pSceneHierarchy->Initialise("Data/Resources.xml", "Data/Prefabs.xml");
	m_pSceneHierarchy->LoadSceneFromXML("Data/Levels/level1.xml");
	
	
	//Load the shader files
	LoadShaders();
	//Load resources from editor data
	m_pResourceManager = new CResourceManager();
	m_pResourceManager->Initialise(this, m_pSceneHierarchy, m_pColourShader);
	
	
	//Load the texture files
	LoadTextures();
	//Add lights to lightmanager
	CreateLights();

	//Create in game entities
	CreateEntities();
	
	return true;
}
void 
COpenGLRenderer::PreDraw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	//Send shader information
	m_pColourShader->SetShader(this);
	//Lighting variables
	SendLightData();
}
void 
COpenGLRenderer::Draw(CCamera* _pCurrentCamera)
{	
	m_pColourShader->SetShaderMatrix(this, "worldMatrix", m_matWorld.m);
	m_pColourShader->SetShaderMatrix(this, "viewMatrix", _pCurrentCamera->GetViewMatrix().m);
	m_pColourShader->SetShaderMatrix(this, "projectionMatrix", _pCurrentCamera->GetProjectionMatrix().m);
	
	DrawLine(m_pShip->GetPosition(), m_pShip->GetPosition() + (m_pShip->GetForward() * 5.0f), TVector3(1.0f, 0.0f, 0.0f));

	//Camera variables
	m_pColourShader->SetShaderVector3(this, "gCameraPosition", _pCurrentCamera->GetPosition());

	
	//Terrain specific variables
	if(m_pTerrain->GetNormalMap() != TEXTURE_INVALID)
	{
		m_pColourShader->SetShaderInteger(this, "gNormalMapTexture", m_pTerrain->GetNormalMap());
	}
	m_pColourShader->SetShaderInteger(this, "gShaderTexture", m_pTerrain->GetTextureType());
	m_pColourShader->SetShaderMatrix(this, "worldMatrix", m_pTerrain->GetWorldMatrix()->m);
	m_pTerrain->Draw(this, _pCurrentCamera);
	// specific variables
	if(m_pModelCollection[MODEL_LOOP].GetNormalMap() != TEXTURE_INVALID)
	{
		m_pColourShader->SetShaderInteger(this, "gNormalMapTexture", m_pModelCollection->GetNormalMap());
	}
	m_pColourShader->SetShaderInteger(this, "gShaderTexture", m_pModelCollection[MODEL_LOOP].GetTextureType());
	m_pColourShader->SetShaderMatrix(this, "worldMatrix", m_pModelCollection[MODEL_LOOP].GetWorldMatrix()->m);
	m_pModelCollection[MODEL_LOOP].Draw(this, _pCurrentCamera);

	m_pHelicopter->Draw(this, _pCurrentCamera, m_pColourShader);

	//Ship specific variables
	m_pColourShader->SetShaderInteger(this, "gNormalMapTexture", m_pModelCollection[MODEL_SHIP].GetNormalMap());
	m_pColourShader->SetShaderInteger(this, "gShaderTexture", m_pModelCollection[MODEL_SHIP].GetTextureType());
	m_pShip->Draw(this, _pCurrentCamera, m_pColourShader);	

	//Draw editor objects
	TEntityNode* pRootNode = m_pSceneHierarchy->GetRootNode();
	for (unsigned int iChild = 0; iChild < pRootNode->vecChildren.size(); ++iChild)
	{
		TEntityNode* pNode = pRootNode->vecChildren[iChild];
		TPrefabDefinition* pPrefab = m_pSceneHierarchy->GetPrefabDefinition(pNode->tEntity.sPrefabName);
		CModel* pModel = m_pResourceManager->GetModel( pPrefab->sModel );
		if (pModel)
		{
			CTexture* pTexture = m_pResourceManager->GetTexture(pPrefab->sTexture);

			TMatrix worldMat;
			float fWorldScale = 0.1f;
			NMatrix::Transformation(worldMat,
									TVector3(pNode->tEntity.vecPosition[0], pNode->tEntity.vecPosition[1], pNode->tEntity.vecPosition[2]),
									TVector3(pNode->tEntity.vecScale[0], pNode->tEntity.vecScale[1], pNode->tEntity.vecScale[2]) * fWorldScale,
									TVector3(pNode->tEntity.vecRotation[0], pNode->tEntity.vecRotation[1], pNode->tEntity.vecRotation[2]));
			m_pColourShader->SetShaderInteger(this, "gShaderTexture", pTexture->GetTextureID());
			m_pColourShader->SetShaderMatrix(this, "worldMatrix", worldMat.m);

			pModel->Draw(this, _pCurrentCamera);
		}
	}
}
void 
COpenGLRenderer::PostDraw()
{
	SwapBuffers(m_hDevContext);
}
void 
COpenGLRenderer::Process(float _fDeltaTick)
{
	ProcessInput();

	m_fGameTime += _fDeltaTick;
	m_pModelCollection[MODEL_LOOP].Process(_fDeltaTick);

	m_pTerrain->Process(_fDeltaTick);
	//m_pHelicopter->SetPosition(TVector3(m_pInput->fMouseX, m_pInput->fMouseY, 0.0f));
	m_pHelicopter->Process(_fDeltaTick);
	//m_pHelicopter->ProcessInput(_fDeltaTick);
	m_pShip->ProcessInput(_fDeltaTick);
	m_pShip->Process(_fDeltaTick);
	m_pLightManager->GetSpot(0)->ProcessParent(_fDeltaTick);

	m_pPerspectiveCamera->Process(_fDeltaTick);
	m_pPerspectiveCamera->ProcessInput(_fDeltaTick);
}
void 
COpenGLRenderer::ProcessInput()
{
	if(m_pInput->bSpace)
	{
		if(m_bIsFirstPerson)
		{
			m_bIsFirstPerson = false;
			m_pPerspectiveCamera->SetParent(m_pShip, &TVector3(0.0f, 0.0f, 0.23f)); //First person
		}
		else
		{
			m_bIsFirstPerson = true;
			m_pPerspectiveCamera->SetParent(m_pShip, &TVector3(0.0f, 0.8f, -4.0f)); //Third person
		}
	}
}
void 
COpenGLRenderer::SetupOpenGL(HWND _hWnd)
{
	m_hDevContext = GetDC(m_hWindow);
	
	int iPixelFormat = 0;
	PIXELFORMATDESCRIPTOR pFormat;
	ZeroMemory(&pFormat, sizeof(PIXELFORMATDESCRIPTOR));

	pFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pFormat.nVersion = 1;
	pFormat.iLayerType = PFD_MAIN_PLANE;
	pFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pFormat.cDepthBits = 32;
	pFormat.cColorBits = 24;
	pFormat.iPixelType = PFD_TYPE_RGBA;
	
	iPixelFormat = ChoosePixelFormat(m_hDevContext, &pFormat);
	SetPixelFormat(m_hDevContext, iPixelFormat, &pFormat);
	m_hRenderContext = wglCreateContext(m_hDevContext);
	wglMakeCurrent(m_hDevContext, m_hRenderContext);

	//Load openGl extensions
	LoadExtensions();

	//Setup OpenGL for Rendering
	m_fFOV = 45.0f;
	m_fAspectRatio = (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Setup matrices
	NMatrix::Identity(m_matWorld);	
}
void 
COpenGLRenderer::CreateEntities()
{
	//Initialise camera objects
	m_pPerspectiveCamera = new CCamera();
	m_pPerspectiveCamera->Initialise(m_fFOV, m_fAspectRatio, 0.01f, 1000.0f, 100.0f, 50.0f, m_iWindowWidth, m_iWindowHeight, CAMERA_PERSPECTIVE);
	m_pPerspectiveCamera->SetInput(m_pInput);
	m_pPerspectiveCamera->SetPosition(TVector3(0.0f, 2.0f, -5.0f));
	m_pOrthographicCamera = new CCamera();
	m_pOrthographicCamera->Initialise(m_fFOV, m_fAspectRatio, 0.1f, 1000.0f, 5.0f, 2.0f, m_iWindowWidth, m_iWindowHeight, CAMERA_ORTHOGONAL);
	m_pOrthographicCamera->SetPosition(TVector3(10.0f, 0.0f, 0.0f));
	m_pOrthographicCamera->SetForward(TVector3(-1.0f, 0.0f, 0.0f));

	//Initialise models
	CreateModels();

	m_pScreenSurface = new CModel();
	m_pScreenSurface->Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 10.0f, TEXTURE_SQUARES, m_pColourShader);
	m_pScreenSurface->LoadSquare();

	m_pTerrain = new CTerrain();
	m_pTerrain->Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 100.0f, TEXTURE_FLOORTILE, m_pColourShader);
	m_pTerrain->LoadTerrain(10, 50.0f);
	m_pTerrain->SetNormalMap(TEXTURE_FLOORNORMAL);

	m_pHelicopter = new CHelicopter();
	m_pHelicopter->Initialise(&m_pModelCollection[MODEL_HELICHASSIS], &m_pModelCollection[MODEL_HELIROTOR], m_pInput);
	m_pHelicopter->SetPosition(TVector3(0.0f, 1.0f, -10.0f));
	
	//m_pPerspectiveCamera->SetParent(m_pHelicopter, &TVector3(0.0f, 0.8f, -4.0f)); //Third person
	//m_pPerspectiveCamera->SetParent(m_pHelicopter, &TVector3(0.0f, 0.2f, 0.3f)); //First person
	m_pShip = new CShip();
	m_pShip->Initialise(&m_pModelCollection[MODEL_SHIP], m_pInput);
	m_pShip->SetPosition(TVector3(0.0f, 2.0f, -10.0f));
	m_pLightManager->GetSpot(0)->SetParent(m_pShip, &TVector3(0.0f, 0.0f, 1.0f));
	m_pPerspectiveCamera->SetParent(m_pShip, &TVector3(0.0f, 0.8f, -4.0f)); //Third person
	//m_pPerspectiveCamera->SetParent(m_pShip, &TVector3(0.0f, 0.0f, 0.2f)); //First person

	//Create Frame Buffer Objects
	//m_pFrameBuffer = new CFrameBufferObject();
	//m_pFrameBuffer->Initialise(m_iWindowWidth, m_iWindowHeight);
	
}
void
COpenGLRenderer::CreateModels()
{
	//Initialise all models used in this scene
	m_pModelCollection = new CModel[MODEL_MAX];

	m_pModelCollection[MODEL_LOOP].Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 10.0f, TEXTURE_STONE, m_pColourShader);
	m_pModelCollection[MODEL_LOOP].LoadFromOBJ("Assets/checkerLoop.obj");
	m_pModelCollection[MODEL_LOOP].SetNormalMap(TEXTURE_STONENORMAL);
	m_pModelCollection[MODEL_LOOP].SetScale(TVector3(10.0f, 10.0f, 10.0f));

	m_pModelCollection[MODEL_HELICHASSIS].Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_STONE, m_pColourShader);
	m_pModelCollection[MODEL_HELICHASSIS].LoadFromOBJ("Assets/helicopter.obj");
	m_pModelCollection[MODEL_HELICHASSIS].SetNormalMap(TEXTURE_STONENORMAL);

	m_pModelCollection[MODEL_HELIROTOR].Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_STONE, m_pColourShader);
	m_pModelCollection[MODEL_HELIROTOR].LoadFromOBJ("Assets/rotor.obj");
	m_pModelCollection[MODEL_HELIROTOR].SetNormalMap(TEXTURE_STONENORMAL);

	m_pModelCollection[MODEL_SHIP].Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_SHIP, m_pColourShader);
	m_pModelCollection[MODEL_SHIP].LoadFromOBJ("Assets/ship.obj");
	m_pModelCollection[MODEL_SHIP].SetNormalMap(TEXTURE_SHIPNORMAL);

	m_pModelCollection[MODEL_CUBE].Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_STONE, m_pColourShader);
	m_pModelCollection[MODEL_CUBE].LoadFromOBJ("Assets/cube.obj");
	m_pModelCollection[MODEL_CUBE].SetNormalMap(TEXTURE_STONENORMAL);
	
}
void 
COpenGLRenderer::LoadShaders()
{
	m_pColourShader = new CShader();
	m_pColourShader->InitialiseShader(this, "Shaders/diffuselighting.vert", "Shaders/diffuselighting.frag");
}
void 
COpenGLRenderer::CleanUp()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hRenderContext);
	ReleaseDC(m_hWindow, m_hDevContext);
}
void 
COpenGLRenderer::ExecuteOneFrame(float _fDeltaTick)
{
	//Process calculations
	Process(_fDeltaTick);

	//Render scene to screen
	PreDraw();
	
	//m_pFrameBuffer->SetToWrite();
	//glClear(GL_DEPTH_BUFFER_BIT);
	Draw(m_pPerspectiveCamera);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//m_pColourShader->SetShaderInteger(this, "gDepthTexture", 0);
	//m_pFrameBuffer->SetToRead(GL_TEXTURE0);
	//Draw();	
	
	PostDraw();
}
//Draw debug line to window
void 
COpenGLRenderer::DrawLine(TVector3& _rStart, TVector3& _rEnd, TVector3& _rColour)
{
	glBegin(GL_LINES);
	glLineWidth(5.5); 
	glColor3f(_rColour.fX, _rColour.fY, _rColour.fZ);
	glVertex3f(_rStart.fX, _rStart.fY, _rStart.fZ);
	glVertex3f(_rEnd.fX, _rEnd.fY, _rEnd.fZ);
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
	m_vecAmbientLight = TVector4(0.0f, 0.0f, 0.0f, 1.0f);

	//Scene lights
	m_pLightManager = new CLightManager();
	m_pLightManager->Initialise();
	
	//Directional
	m_pLightManager->AddDirectional(TVector3(1.0f, -0.1f, 0.0f), TVector4(0.5f, 0.5f, 0.55f, 1.0f), 500.0f);
	//Point
	m_pLightManager->AddPoint(TVector3(-5.0f, 5.0f, 0.0f), TVector4(1.0f, 0.2f, 0.2f, 1.0f), TVector3(1.0f, 0.5f, 0.1f), 500.1f);
	m_pLightManager->AddPoint(TVector3(5.0f, 5.0f, 0.0f), TVector4(0.2f, 1.0f, 0.2f, 1.0f), TVector3(1.0f, 0.5f, 0.1f), 500.1f);
	//Spot
	m_pLightManager->AddSpot(TVector3(0.0f, 5.0f, -5.0f), TVector3(0.0f, 5.0f, 5.0f) * -1.0f, TVector4(3.0f, 3.0f, 5.0f, 1.0f), TVector3(1.0f, 0.5f, 0.2f), 0.6f, 500.0f);
	//m_pLightManager->AddSpot(TVector3(0.0f, 5.0f, 5.0f), TVector3(0.0f, 5.0f, 5.0f) * -1.0f, TVector4(0.2f, 0.2f, 0.2f, 1.0f), TVector3(1.0f, 0.5f, 0.2f), 0.5f, 50.0f);
}
void 
COpenGLRenderer::LoadExtensions()
{
	// Load the OpenGL extensions that this application will be using.
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
}