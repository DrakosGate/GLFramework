

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "component.h"

class GameObject;

namespace Component
{
	class Transform : public BaseComponent
	{
	public:
		Transform( );
		~Transform( );

		virtual bool Initialise( GameObject* _pOwner );
		virtual void Process( float _fDeltaTime );
		virtual const char* GetName( ) override { return "Transform"; }

		//Parent functions
		virtual void AddChild( GameObject* _pChild );
		virtual void SetParent( GameObject* _pParent );
		
		//PRS Accessor functions
		virtual void SetPosition( glm::vec3& _rVecPosition );
		virtual glm::vec3& GetPosition( );
		virtual void SetRotation( glm::vec3& _rRotation );
		virtual glm::vec3& GetRotation( );
		virtual void SetScale( glm::vec3& _rScale );
		virtual glm::vec3& GetScale( );

		//Vector Accessor functions
		virtual void SetForward( glm::vec3& _rVecPosition );
		virtual glm::vec3& GetForward( );
		virtual void SetRight( glm::vec3& _rVecRight );
		virtual glm::vec3& GetRight( );
		virtual void SetUp( glm::vec3& _rVecUp );
		virtual glm::vec3& GetUp( );

	protected:
		glm::mat4x4 m_matWorld;

		glm::vec3 m_vecPosition;
		glm::vec3 m_vecScale;
		glm::vec3 m_vecRotation;

		glm::vec3 m_vecForward;
		glm::vec3 m_vecRight;
		glm::vec3 m_vecUp;

		GameObject* m_pParent;
		std::vector< GameObject* > m_vecChildren;

	};
}