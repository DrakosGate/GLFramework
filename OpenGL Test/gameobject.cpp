
// Library Includes
#include <glm/gtc/matrix_transform.hpp>

// Local Includes
#include "component.h"
#include "defines.h"

// This Include
#include "gameobject.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CGameObject constructor
*
* @author Christopher Howlett
*
*/
GameObject::GameObject()
{
	
}

/**
*
* CGameObject destructor
*
* @author Christopher Howlett
*
*/
GameObject::~GameObject()
{

}
/**
*
* CGameObject Initialise
*
* @author Christopher Howlett
*
*/
bool 
GameObject::Initialise()
{
	return true;
}
void 
GameObject::Process( const float _fDeltaTime )
{
	m_transform.Process( _fDeltaTime );
}

void
GameObject::AddChild( GameObject* _pChild )
{
	m_transform.AddChild( _pChild );
}

void
GameObject::SetParent( GameObject* _pParent )
{
	m_transform.SetParent( _pParent );
}

void
GameObject::AddComponent( BaseComponent* _pComponent )
{
	m_vecComponents.push_back( _pComponent );
}

BaseComponent*
GameObject::GetComponentByName( char* _pName )
{
	for ( ComponentConstIterator iter = m_vecComponents.begin( ); iter != m_vecComponents.end( ); ++iter )
	{
		if ( strcmp( (*iter)->GetName( ), _pName ) == 0 )
		{
			return (*iter );
		}
	}
	return 0;
}

///**
//*
//* CGameObject Draw
//*
//* @author Christopher Howlett
//* @param _pRenderer OpenGL Renderer
//* @param _pCamera Camera used for rendering
//*
//*/
//void 
//GameObject::Draw(OpenGLRenderer* _pRenderer, Camera* _pCamera)
//{
//	//Do nothing for this class
//}
