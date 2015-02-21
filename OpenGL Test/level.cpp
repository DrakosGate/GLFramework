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
#include "window.h"
#include "camera.h"

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
: m_pPerspectiveCamera( 0 )
, m_pOrthographicCamera( 0 )
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
	SAFEDELETE( m_pPerspectiveCamera );
	SAFEDELETE( m_pOrthographicCamera );

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

	m_pPerspectiveCamera = new Camera( );
	m_pPerspectiveCamera->Initialise( m_fFOV, m_fAspectRatio, 0.01f, 1000.0f, 100.0f, 50.0f, _pWindow->GetWindowWidth(), _pWindow->GetWindowHeight(), CAMERA_PERSPECTIVE );
	m_pPerspectiveCamera->GetTransform().SetPosition(glm::vec3(0.0f, 2.0f, -5.0f));
	m_pOrthographicCamera = new Camera( );
	m_pOrthographicCamera->Initialise( m_fFOV, m_fAspectRatio, 0.1f, 1000.0f, 5.0f, 2.0f, _pWindow->GetWindowWidth( ), _pWindow->GetWindowHeight( ), CAMERA_ORTHOGONAL );
	m_pOrthographicCamera->GetTransform().SetPosition( glm::vec3( 10.0f, 0.0f, 0.0f ) );
	m_pOrthographicCamera->GetTransform( ).SetForward( glm::vec3( -1.0f, 0.0f, 0.0f ) );

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
	m_pPerspectiveCamera->Process( _fDeltaTime );
}