

#pragma once

#include <windows.h>
#include <glm/glm.hpp>

namespace Listeners
{
	
	class InputListener
	{
	public:
		InputListener( ){}
		~InputListener( ){}

		virtual void OnKeyDown( const unsigned int _iKeyID, const float _fDeltaTime ) {}
		virtual void OnKeyUp( const unsigned int _iKeyID, const float _fDeltaTime ) {}
		virtual void OnMouseButtonDown( const unsigned int _iKeyID, const float _fDeltaTime ) {}
		virtual void OnMouseButtonUp( const unsigned int _iKeyID, const float _fDeltaTime ) {}
		virtual void OnMouseMove( glm::vec2& _offset, const float _fDeltaTime ) {}
	};

}