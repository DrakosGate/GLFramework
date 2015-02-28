//
// (c) 2013 Media Design School
//
//  File Name   :   Level.cpp
//  Description :   Level Interface
//  Author      :   Chris Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "openglrenderer.h"
#include "renderablecomponent.h"
#include "window.h"
#include "camera.h"
#include "shader.h"
#include "inputmanager.h"
#include "resourcemanager.h"

// This Include
#include "Level.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* Level class Constructor
*
* @author Christopher Howlett
*
*/
Level::Level( )
{

}

/**
*
* Level class Destructor
*
* @author Christopher Howlett
*
*/
Level::~Level()
{
	InputManager::GetInstance()->Delete();
}

/**
*
* Level class Initialisation
*
* @author Christopher Howlett
*
*/
bool
Level::Initialise( OpenGLRenderer* _pRenderer, Window* _pWindow )
{
	bool bResult = true;
	const float m_fFOV = 45.0f;
	const float m_fAspectRatio = (GLfloat) _pWindow->GetWindowWidth( ) / (GLfloat) _pWindow->GetWindowHeight( );

	m_pPerspectiveCamera.Initialise( m_fFOV, m_fAspectRatio, 0.01f, 1000.0f, 100.0f, 50.0f, _pWindow->GetWindowWidth( ), _pWindow->GetWindowHeight( ), CAMERA_PERSPECTIVE );
	m_pPerspectiveCamera.GetTransform( ).SetPosition( glm::vec3( 0.0f, 2.0f, -15.0f ) );
	m_pOrthographiCamera.Initialise( m_fFOV, m_fAspectRatio, 0.1f, 1000.0f, 5.0f, 2.0f, _pWindow->GetWindowWidth( ), _pWindow->GetWindowHeight( ), CAMERA_ORTHOGONAL );
	m_pOrthographiCamera.GetTransform( ).SetPosition( glm::vec3( 10.0f, 0.0f, 0.0f ) );
	m_pOrthographiCamera.GetTransform( ).SetForward( glm::vec3( -1.0f, 0.0f, 0.0f ) );


	pShader.InitialiseShader( "Shaders/diffuselighting.vert", "Shaders/diffuselighting.frag" );

	m_resourceManager.Initialise( "Data/Resources.xml" );

	testobject.Initialise( );
	testobject.GetTransform( ).SetPosition( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	testobject.GetTransform( ).SetRotation( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	Component::Renderable* pRenderable = testobject.AddComponent<Component::Renderable>( );
	pRenderable->Initialise( &testobject, 
							 m_resourceManager.GetModel( std::string( "chicken" ) ),
							 m_resourceManager.GetTexture( std::string( "chicken" ) ),
							 &pShader );
	InputManager::GetInstance( )->AddListener( &testobject );


	SetFocus( _pWindow->GetHandle() );
	SetForegroundWindow( _pWindow->GetHandle() );

	return bResult;
}
/**
*
* Level class Process
*
* @author Christopher Howlett
*
*/
void
Level::Process( const float _fDeltaTime )
{
	m_pPerspectiveCamera.Process( _fDeltaTime );
	testobject.GetTransform( ).SetRotation( testobject.GetTransform( ).GetRotation( ) + glm::vec3( 0.0f, _fDeltaTime, 0.0f ) );
	testobject.GetTransform( ).SetPosition( glm::vec3( sinf( testobject.GetTransform( ).GetRotation( ).y ), 0.0f, 0.0f ) );
	testobject.GetTransform( ).SetScale( glm::vec3( 5.0f, 5.0f, 5.0f ) );
	testobject.Process( _fDeltaTime );
}
/**
*
* Level class Draw
*
* @author Christopher Howlett
*
*/
void
Level::Draw( )
{
	//Send shader information
	//Lighting variables
	//SendLightData();

	Component::Renderable* rendercomp = testobject.GetComponent<Component::Renderable>();
	
	rendercomp->GetShader( )->SetShader( );
	int texid = rendercomp->GetTexture( )->GetResourceID( );
	rendercomp->GetShader( )->SetShaderInteger( "gShaderTexture", texid );
	
	rendercomp->GetShader( )->SetShaderMatrix( "worldMatrix", testobject.GetTransform( ).GetMatrix( ) );
	rendercomp->GetShader( )->SetShaderMatrix( "viewMatrix", m_pPerspectiveCamera.GetViewMatrix( ) );
	rendercomp->GetShader( )->SetShaderMatrix( "projectionMatrix", m_pPerspectiveCamera.GetProjectionMatrix( ) );

	rendercomp->Draw( );
	/*
	//DrawLine(m_pShip->GetPosition(), m_pShip->GetPosition() + (m_pShip->GetForward() * 5.0f), glm::vec3(1.0f, 0.0f, 0.0f));

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
	//if(m_pModelCollection[MODEL_LOOP].GetNormalMap() != TEXTURE_INVALID)
	//{
	//	m_pColourShader->SetShaderInteger(this, "gNormalMapTexture", m_pModelCollection->GetNormalMap());
	//}
	//m_pColourShader->SetShaderInteger(this, "gShaderTexture", m_pModelCollection[MODEL_LOOP].GetTextureType());
	//m_pColourShader->SetShaderMatrix(this, "worldMatrix", m_pModelCollection[MODEL_LOOP].GetWorldMatrix()->m);
	//m_pModelCollection[MODEL_LOOP].Draw(this, _pCurrentCamera);
	//
	//m_pHelicopter->Draw(this, _pCurrentCamera, m_pColourShader);
	//
	////Ship specific variables
	//m_pColourShader->SetShaderInteger(this, "gNormalMapTexture", m_pModelCollection[MODEL_SHIP].GetNormalMap());
	//m_pColourShader->SetShaderInteger(this, "gShaderTexture", m_pModelCollection[MODEL_SHIP].GetTextureType());
	//m_pShip->Draw(this, _pCurrentCamera, m_pColourShader);

	//Draw editor objects
	TSceneNode* pRootNode = m_pSceneHierarchy->GetRootNode();
	m_pColourShader->SetShaderInteger(this, "gNormalMapTexture", m_pTextureCollection[TEXTURE_STONENORMAL].GetTextureID());
	for (unsigned int iChild = 0; iChild < pRootNode->vecChildren.size(); ++iChild)
	{
	TSceneNode* pNode = pRootNode->vecChildren[iChild];
	TPrefabDefinition* pPrefab = m_pSceneHierarchy->GetPrefabDefinition(pNode->tEntity.sPrefabName);
	if (pPrefab)
	{
	CModel* pModel = m_pResourceManager->GetModel(pPrefab->sModel);
	if (pModel)
	{
	CTexture* pTexture = m_pResourceManager->GetTexture(pPrefab->sTexture);

	glm::mat4 worldMat;
	float fWorldScale = 0.1f;
	NMatrix::Transformation(worldMat,
	glm::vec3(pNode->tEntity.vecPosition[0], pNode->tEntity.vecPosition[1], pNode->tEntity.vecPosition[2]),
	glm::vec3(pNode->tEntity.vecScale[0], pNode->tEntity.vecScale[1], pNode->tEntity.vecScale[2]) * fWorldScale,
	glm::vec3(pNode->tEntity.vecRotation[0], pNode->tEntity.vecRotation[1], pNode->tEntity.vecRotation[2]));
	m_pColourShader->SetShaderInteger(this, "gShaderTexture", pTexture->GetTextureID());
	m_pColourShader->SetShaderMatrix(this, "worldMatrix", worldMat.m);

	pModel->Draw(this, _pCurrentCamera);
	}
	}
	}*/
}