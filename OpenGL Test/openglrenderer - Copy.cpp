//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
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

// This Include
#include "openglrenderer.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* COpenGLRenderer Constructor
*
* @author Christopher Howlett
*
*/
COpenGLRenderer::COpenGLRenderer()
:	m_hWindow(0)
,	m_hDevContext(0)
,	m_fFOV(0)
,	m_fAspectRatio(0)
,	m_pPerspectiveCamera(0)
,	m_pModelCollection(0)
,	m_pColourShader(0)
,	m_pInput(0)
,	m_pTextureCollection(0)
,	m_pConsole(0)
,	m_pLightManager(0)
,	m_pTerrain(0)
,	m_fGameTime(0.0f)
,	m_pFrameBuffer(0)
,	m_fOrthoCameraZoom(0.0f)
,	m_hFont(0)
,	m_hOldFont(0)
,	m_iCurrentTextMessage(0)
,	m_bDoDrawControls(false)
,	m_pHuman(0)
,	m_pTree(0)
,	m_iNumTrees(0)
{

}
/**
*
* COpenGLRenderer Destructor
*
* @author Christopher Howlett
*
*/
COpenGLRenderer::~COpenGLRenderer()
{
	CleanupFonts();
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
	if(m_pHuman)
	{
		delete m_pHuman;
		m_pHuman = 0;
	}
	if(m_pTree)
	{
		delete[] m_pTree;
		m_pTree = 0;
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
/**
*
* COpenGLRenderer Initialisation
*
* @author Christopher Howlett
* @param _hWnd Handle to window
* @param _iWindowWidth Window width
* @param _iWindowHeight Window Height
* @param _pInput Struct used for user input
*
*/
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
	//m_pConsole->InitialiseConsole();
	
	//Initialise OpenGL 
	SetupOpenGL(_hWnd);

	//Load the shader files
	LoadShaders();
	//Load the texture files
	LoadTextures();
	//Add lights to lightmanager
	CreateLights();
	//Create bitmap font
	SetupFont(L"Verdana");

	
	//Create in game entities
	CreateEntities();
	
	//Once initialisation is complete, return focus to main window
	//m_pConsole->ShutdownConsole();

	return true;
}
/**
*
* COpenGLRenderer SetupOpenGL
*
* @author Christopher Howlett
* @param _hWnd Handle to window used for OpenGl
*
*/
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
	m_fAspectRatio = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Setup matrices
	NMatrix::Identity(m_matWorld);	
}
/**
*
* COpenGLRenderer CreateEntities
*
* @author Christopher Howlett
*
*/
void 
COpenGLRenderer::CreateEntities()
{
	//Initialise camera objects
	m_fOrthoCameraZoom = 1.0f;
	m_pPerspectiveCamera = new CCamera();
	m_pPerspectiveCamera->Initialise(m_fFOV, m_fAspectRatio, 0.01f, 1000.0f, 60.0f, 15.0f, m_iWindowWidth, m_iWindowHeight, CAMERA_PERSPECTIVE);
	m_pPerspectiveCamera->SetInput(m_pInput);
	m_pPerspectiveCamera->SetPosition(TVector3(0.0f, 5.0f, -15.0f));
	m_pPerspectiveCamera->SetForward(TVector3(0.0f, 0.0f, 1.0f));
	m_pOrthographicCamera = new CCamera();
	m_pOrthographicCamera->Initialise(m_fFOV, m_fAspectRatio, 0.1f, 1000.0f, 5.0f, 2.0f, static_cast<int>(m_iWindowWidth * m_fOrthoCameraZoom), static_cast<int>(m_iWindowHeight * m_fOrthoCameraZoom), CAMERA_ORTHOGONAL);
	m_pOrthographicCamera->SetPosition(TVector3(0.0f, 0.0f, 10.0f));
	m_pOrthographicCamera->SetForward(TVector3(0.0f, 0.0f, -1.0f));
	
	//Initialise models
	CreateModels();

	//Create game entities
	m_pScreenSurface = new CModel();
	m_pScreenSurface->Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 10.0f, TEXTURE_GRASS, m_pColourShader);
	m_pScreenSurface->LoadSquare();

	m_pTerrain = new CTerrain();
	m_pTerrain->Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 120.0f, TEXTURE_GRASS, m_pColourShader);
	m_pTerrain->LoadTerrain(20, 20.0f);
	m_pTerrain->SetNormalMap(TEXTURE_GRASS);

	m_pHuman = new CModel();
	m_pHuman->Initialise(this, TVector3(1.0f, 0.5f, 0.0f), 2.0f, TEXTURE_HUMAN, m_pColourShader);
	m_pHuman->LoadSquare();
	m_pHuman->SetNormalMap(TEXTURE_HUMAN);

	m_iNumTrees = 20;
	m_pTree = new CModel[m_iNumTrees];
	for(int iTree = 0; iTree < m_iNumTrees; ++iTree)
	{
		m_pTree[iTree].Initialise(this, TVector3(iTree - (m_iNumTrees * 0.5f), 0.75f, iTree - (m_iNumTrees * 0.5f)), 3.0f, TEXTURE_TREE, m_pColourShader);
		m_pTree[iTree].LoadSquare();
		m_pTree[iTree].SetNormalMap(TEXTURE_TREE);
		m_vecEntityCollection.push_back(&m_pTree[iTree]);
	}
		
	//Add to collection
	//m_vecEntityCollection.push_back(m_pCursor);
	m_vecEntityCollection.push_back(m_pTerrain);
	m_vecEntityCollection.push_back(m_pHuman);
	
	
	//Create Frame Buffer Objects
	m_pFrameBuffer = new CFrameBufferObject();
	m_pFrameBuffer->Initialise(m_iWindowWidth, m_iWindowHeight);
	m_pLightManager->GetSpot(0)->SetParent(m_pPerspectiveCamera, &TVector3(0.0f, 0.0f, 1.0f));
}
/**
*
* COpenGLRenderer CreateModels
*
* @author Christopher Howlett
*
*/
void
COpenGLRenderer::CreateModels()
{
	//Initialise all models used in this scene
	//m_pModelCollection = new CModel[MODEL_MAX];
	//
	//m_pModelCollection[MODEL_LOOP].Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 10.0f, TEXTURE_STONE, m_pColourShader);
	//m_pModelCollection[MODEL_LOOP].LoadFromOBJ("Assets/checkerLoop.obj");
	//m_pModelCollection[MODEL_LOOP].SetNormalMap(TEXTURE_STONENORMAL);
	//m_pModelCollection[MODEL_LOOP].SetScale(TVector3(10.0f, 10.0f, 10.0f));
	//
	//m_pModelCollection[MODEL_HELICHASSIS].Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_STONE, m_pColourShader);
	//m_pModelCollection[MODEL_HELICHASSIS].LoadFromOBJ("Assets/helicopter.obj");
	//m_pModelCollection[MODEL_HELICHASSIS].SetNormalMap(TEXTURE_STONENORMAL);
	//
	//m_pModelCollection[MODEL_HELIROTOR].Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_STONE, m_pColourShader);
	//m_pModelCollection[MODEL_HELIROTOR].LoadFromOBJ("Assets/rotor.obj");
	//m_pModelCollection[MODEL_HELIROTOR].SetNormalMap(TEXTURE_STONENORMAL);
	//
	//m_pModelCollection[MODEL_SHIP].Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_SHIP, m_pColourShader);
	//m_pModelCollection[MODEL_SHIP].LoadFromOBJ("Assets/ship.obj");
	//m_pModelCollection[MODEL_SHIP].SetNormalMap(TEXTURE_SHIPNORMAL);
	//
	//m_pModelCollection[MODEL_CUBE].Initialise(this, TVector3(0.0f, 0.0f, 0.0f), 1.0f, TEXTURE_STONE, m_pColourShader);
	//m_pModelCollection[MODEL_CUBE].LoadFromOBJ("Assets/cube.obj");
	//m_pModelCollection[MODEL_CUBE].SetNormalMap(TEXTURE_STONENORMAL);
}
/**
*
* COpenGLRenderer LoadShaders
*
* @author Christopher Howlett
*
*/
void 
COpenGLRenderer::LoadShaders()
{
	m_pColourShader = new CShader();
	m_pColourShader->InitialiseShader(this, "Shaders/diffuselighting.vert", "Shaders/diffuselighting.frag");
}
/**
*
* COpenGLRenderer LoadTextures
*
* @author Christopher Howlett
*
*/
void 
COpenGLRenderer::LoadTextures()
{
	m_pTextureCollection = new CTexture[TEXTURE_MAX];
	m_pTextureCollection[TEXTURE_STONE].Initialise(this, "Assets/stone.tga", TEXTURE_STONE);
	m_pTextureCollection[TEXTURE_STONENORMAL].Initialise(this, "Assets/stoneNormals.tga", TEXTURE_STONENORMAL);
	//Sprites
	m_pTextureCollection[TEXTURE_HUMAN].Initialise(this,	"Assets/Sprites/human.tga", TEXTURE_HUMAN);
	m_pTextureCollection[TEXTURE_TREE].Initialise(this,		"Assets/Sprites/tree.tga", TEXTURE_TREE);
	m_pTextureCollection[TEXTURE_GRASS].Initialise(this,	"Assets/Sprites/grass.tga", TEXTURE_GRASS);
}
/**
*
* COpenGLRenderer CreateLights
*
* @author Christopher Howlett
*
*/
void 
COpenGLRenderer::CreateLights()
{
	//Ambient light
	m_vecAmbientLight = TVector4(0.5f, 0.5f, 0.5f, 1.0f);

	//Scene lights
	m_pLightManager = new CLightManager();
	m_pLightManager->Initialise();
	
	//Directional
	m_pLightManager->AddDirectional(TVector3(0.0f, -0.2f, 1.0f), TVector4(0.5f, 0.6f, 0.5f, 1.0f), 5.0f);
	//Point
	//m_pLightManager->AddPoint(TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.5f, 0.02f), 500.0f);
	//Spot
	//m_pLightManager->AddSpot(TVector3(0.0f, 5.0f, 15.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector3(1.0f, 0.5f, 0.02f), 0.5f, 500.0f);
}
/**
*
* COpenGLRenderer ExecuteOneFrame
*
* @author Christopher Howlett
* @param _fDeltaTick Game time elapsed
*
*/
void 
COpenGLRenderer::ExecuteOneFrame(float _fDeltaTick)
{
	//Process calculations
	Process(_fDeltaTick);

	//Render scene to screen
	PreDraw();
	
	//Draw scene and font
	Draw(m_pPerspectiveCamera);
	PrepareFont();
	
	PostDraw();
}
/**
*
* COpenGLRenderer Process
*
* @author Christopher Howlett
* @param _fDeltaTick Game time elapsed
*
*/
void 
COpenGLRenderer::Process(float _fDeltaTick)
{
	ProcessInput(_fDeltaTick);
	
	m_fGameTime += _fDeltaTick;
	
	//Process billboard
	for(int iTree = 0; iTree < m_iNumTrees; ++iTree)
	{
		TVector3 vecToCam = m_pTree[iTree].GetPosition() - m_pPerspectiveCamera->GetPosition();
		vecToCam.Normalise();
		m_pTree[iTree].SetRotation(TVector3(0.0f, 180.0f + atan2(vecToCam.fZ, vecToCam.fX), 0.0f));
	}	

	m_pPerspectiveCamera->Process(_fDeltaTick);
	m_pPerspectiveCamera->ProcessInput(_fDeltaTick);
	m_pOrthographicCamera->Process(_fDeltaTick);

	//Check for 3D mouse coordinates
	float fRayLength = 1000.0f;
	m_pPerspectiveCamera->Calculate3DMouseCoordinates(TVector2(m_pInput->fMouseX, m_pInput->fMouseY), m_pInput->t3DMouseRay);
	
	for(unsigned int iEntity = 0; iEntity < m_vecEntityCollection.size(); ++iEntity)
	{
		m_vecEntityCollection[iEntity]->Process(_fDeltaTick);
	}
}
/**
*
* COpenGLRenderer Process user input
*
* @author Christopher Howlett
* @param _fDeltaTick Game Time elapsed
*
*/
void 
COpenGLRenderer::ProcessInput(float _fDeltaTick)
{
	
}
/**
*
* COpenGLRenderer PreDraw functionality
*
* @author Christopher Howlett
*
*/
void 
COpenGLRenderer::PreDraw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	//Send shader information
	m_pColourShader->SetShader(this);
	//Lighting variables
	SendLightData();
}
/**
*
* COpenGLRenderer Render using assigned camera
*
* @author Christopher Howlett
* @param _pCurrentCamera Camera used for rendering
*
*/
void 
COpenGLRenderer::Draw(CCamera* _pCurrentCamera)
{	
	m_pColourShader->SetShaderMatrix(this, "worldMatrix", m_matWorld.m);
	m_pColourShader->SetShaderMatrix(this, "viewMatrix", _pCurrentCamera->GetViewMatrix().m);
	m_pColourShader->SetShaderMatrix(this, "projectionMatrix", _pCurrentCamera->GetProjectionMatrix().m);
	
	//Camera variables
	m_pColourShader->SetShaderVector3(this, "gCameraPosition", _pCurrentCamera->GetPosition());
	
	//RENDER ALL ENTITIES IN COLLECTION
	for(unsigned int iEntity = 0; iEntity < m_vecEntityCollection.size(); ++iEntity)
	{
		if(m_vecEntityCollection[iEntity]->GetNormalMap() != TEXTURE_INVALID)
		{
			m_pColourShader->SetShaderInteger(this, "gNormalMapTexture", m_vecEntityCollection[iEntity]->GetNormalMap());
		}
		m_pColourShader->SetShaderInteger(this, "gShaderTexture", m_vecEntityCollection[iEntity]->GetTextureType());
		m_pColourShader->SetShaderMatrix(this, "worldMatrix", m_vecEntityCollection[iEntity]->GetWorldMatrix()->m);
		m_vecEntityCollection[iEntity]->Draw(this, _pCurrentCamera);
	}
}
/**
*
* COpenGLRenderer Post Draw functionality
*
* @author Christopher Howlett
*
*/
void 
COpenGLRenderer::PostDraw()
{
	SwapBuffers(m_hDevContext);
}
/**
*
* COpenGLRenderer Sets the current frames per second count
*
* @author Christopher Howlett
*
*/
void 
COpenGLRenderer::SetFPSCount(int _iFramesPerSecond)
{
	m_iFramesPerSecond = _iFramesPerSecond;
}
/**
*
* COpenGLRenderer Cleans up OpenGL
*
* @author Christopher Howlett
*
*/
void 
COpenGLRenderer::CleanUp()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hRenderContext);
	ReleaseDC(m_hWindow, m_hDevContext);
}
/**
*
* COpenGLRenderer Prepares OpenGL for font drawing
*
* @author Christopher Howlett
* @param _pcFontFilename Name of font
*
*/
void 
COpenGLRenderer::PrepareFont()
{
	m_iCurrentTextMessage = 0;

	//Reset shader variables
	m_pColourShader->SetShaderVector4(this, "gAmbientLight", TVector4(100.0f, 100.0f, 100.0f, 1.0f));
	m_pColourShader->SetShaderMatrix(this, "worldMatrix", m_matWorld.m);
	m_pColourShader->SetShaderMatrix(this, "viewMatrix", m_pOrthographicCamera->GetViewMatrix().m);
	m_pColourShader->SetShaderMatrix(this, "projectionMatrix", m_pOrthographicCamera->GetProjectionMatrix().m);

	glLoadIdentity();

	//Draw text
	if(m_bDoDrawControls)
	{
		
	}
	else
	{
		//Print Frames Per Second
		char textBuffer[64];
		sprintf_s(textBuffer, "FPS: %i", m_iFramesPerSecond);
		PrintText(textBuffer);
	}
}
/**
*
* COpenGLRenderer Creates font for use in OpenGL
*
* @author Christopher Howlett
* @param _pcFontFilename Name of font
*
*/
void 
COpenGLRenderer::SetupFont(wchar_t* _pcFontFilename)
{
	m_iFontSize = -18;
	m_uiFontBase = glGenLists(96); //Store 96 characters
	m_hFont = CreateFont(	m_iFontSize,    //Create font with handle
							0,
							0,
							0,
							FALSE,
							FALSE,
							FALSE,
							FALSE,
							ANSI_CHARSET,
							OUT_TT_PRECIS,
							CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY,
							FF_DONTCARE | DEFAULT_PITCH,
							_pcFontFilename);

	m_hOldFont = static_cast<HFONT>(SelectObject(m_hDevContext, m_hFont));
	wglUseFontBitmaps(m_hDevContext, 32, 96, m_uiFontBase);
	SelectObject(m_hDevContext, m_hOldFont);
	DeleteObject(m_hFont);
}
/**
*
* COpenGLRenderer Renders text to screen using font
*
* @author Christopher Howlett
* @param _pcMessage Text message to draw
*
*/
void 
COpenGLRenderer::PrintText(char* _pcMessage)
{
	char textBuffer[256];
	va_list vecArguments;
	
	//Position text
	glRasterPos2f(m_iWindowWidth * -0.48f, m_iWindowHeight * 0.4f + (m_iCurrentTextMessage * m_iFontSize));
	//Check if message is valid
	if(_pcMessage)
	{
		va_start(vecArguments, _pcMessage);				//Parse string for variables
		vsprintf_s(textBuffer, _pcMessage, vecArguments);	//Convert symbols to numbers
		va_end(vecArguments);							//Store results in text

		glPushAttrib(GL_LIST_BIT);
		glListBase(m_uiFontBase - 32);	//Offset base by 32
		glCallLists(strlen(textBuffer), GL_UNSIGNED_BYTE, textBuffer); //Draw display list text
		glPopAttrib();					//Pop display list bits
	}
	++m_iCurrentTextMessage;
}

/**
*
* COpenGLRenderer Kills all currently active fonts
*
* @author Christopher Howlett
*
*/
void 
COpenGLRenderer::CleanupFonts()
{
	glDeleteLists(m_uiFontBase, 96);
}
/**
*
* COpenGLRenderer Draws Debug Line to screen
*
* @author Christopher Howlett
*
*/
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
/**
*
* COpenGLRenderer Send light data to shader
*
* @author Christopher Howlett
*
*/
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
/**
*
* COpenGLRenderer Load OpenGL Extensions
*
* @author Christopher Howlett
*
*/
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