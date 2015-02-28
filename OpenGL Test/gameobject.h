//
//  File Name   :   gameobject.h
//  Description :   Class of CGameObject
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

// Library Includes
#include <vector>
#include <typeinfo>
#include <glm/glm.hpp>

// Local Includes
#include "transformcomponent.h"
#include "inputlistener.h"

// Types

// Constants

// Prototypes
class OpenGLRenderer;
class Camera;
class BaseComponent;


namespace Component
{
	class Transform;
}

class GameObject : public Listeners::InputListener
{
public:
	GameObject();
	virtual ~GameObject();

	virtual bool Initialise();
	virtual void Process( const float _fDeltaTime );
	virtual Component::Transform& GetTransform( ){ return m_transform; } 

	//Transform functionality
	virtual void AddChild( GameObject* _pChild );
	virtual void SetParent( GameObject* _pParent );

	//Input management
	virtual void OnKeyDown( const unsigned int _iKeyID, const float _fDeltaTime ) override;

	//Component functionality
	virtual void AddComponent( BaseComponent* _pComponent );
	virtual BaseComponent* GetComponentByName( char* _pName );

	template <typename T>
	T* AddComponent( )
	{
		T* pNewComponent = new T();
		AddComponent( pNewComponent );
		return pNewComponent;
	}
	template <typename T>
	T* GetComponent( )
	{
		for ( ComponentConstIterator iter = m_vecComponents.begin( ); iter != m_vecComponents.end( ); ++iter )
		{
			if ( typeid( T ) == typeid( **iter ) )
			{
				return dynamic_cast<T*>( *iter );
			}
		}
		return 0;
	}

	//Useful typedefs
	typedef std::vector< BaseComponent* >::iterator ComponentIterator;
	typedef std::vector< BaseComponent* >::const_iterator ComponentConstIterator;

	//virtual bool Initialise();
	//virtual void Draw(OpenGLRenderer* _pRenderer, Camera* _pCamera);
	//virtual void Process(float _fDeltaTime);
	//
	
	
private:
	GameObject(const GameObject& _kr);
	GameObject& operator= (const GameObject& _rhs);
	
protected:
	//bool m_bHasParent;
	std::vector< BaseComponent* > m_vecComponents;
	Component::Transform m_transform;
};

#endif //__GAMEOBJECT_H__